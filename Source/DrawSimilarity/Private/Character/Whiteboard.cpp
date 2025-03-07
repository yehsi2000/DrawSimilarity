// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Whiteboard.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "DataSaveLoader.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWhiteboard::AWhiteboard()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	WhiteBoard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WhiteBoard"));
	SetRootComponent(WhiteBoard);
	Background = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Background"));
	Background->SetupAttachment(WhiteBoard);
	DataSaveLoader = CreateDefaultSubobject<UDataSaveLoader>(TEXT("DataSaveLoader"));
}

void AWhiteboard::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWhiteboard, DrawArray);
	//DOREPLIFETIME(AWhiteboard, DrawArrayX);
	
}

bool AWhiteboard::IsSupportedForNetworking() const
{
	return true;
}


void AWhiteboard::BeginPlay()
{
	Super::BeginPlay();
	RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, BoardResolution.X, BoardResolution.Y, RTF_RGBA16f, InitialColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, InitialColor);
	auto matInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, Material);
	matInstance->SetTextureParameterValue("RenderTarget", RenderTarget);
	WhiteBoard->SetMaterial(0, matInstance);
	LoadBoard();
}


void AWhiteboard::OnRep_DrawArray()
{
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, InitialColor);
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);
	if (Canvas) {
		FVector2D PrevPoint{ -1,-1 };
		for (int i = 0; i < DrawArray.Num(); i++) {
			if (DrawArray[i].X < 0) {
				PrevPoint = { -1,-1 };
				continue;
			}
			if (PrevPoint.X < 0) {
				PrevPoint = DrawArray[i];
				continue;
			}
			Canvas->K2_DrawLine(PrevPoint, DrawArray[i], BrushThickness, DrawColor);
			PrevPoint = DrawArray[i];
		}
	}
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);

}

void AWhiteboard::DrawLine(FHitResult hit, bool inithit)
{
	FVector2D UV;
	UGameplayStatics::FindCollisionUV(hit, 0, UV);

	if (inithit)
	{
		//first dot cannot make line
		PrevHitUVLoc = BoardResolution * UV;
		DrawArray.Add(FVector2D(-1, -1)); //not actual point, just magic number to indicate new line
	}
	else
	{
		//begin drawing
		UCanvas* Canvas;
		FVector2D Size;
		FDrawToRenderTargetContext Context;
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);
		if (Canvas) {
			Canvas->K2_DrawLine(PrevHitUVLoc, UV * Size, BrushThickness, DrawColor);
		}
		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
		float distToPrevPoint = UKismetMathLibrary::Distance2D(PrevHitUVLoc, UV * Size);
		//UE_LOG(LogTemp, Warning, TEXT("Drawing"));
		if (!UKismetMathLibrary::NearlyEqual_FloatFloat(distToPrevPoint, 0.f))
		{
			PrevHitUVLoc = UV * Size;
			DrawArray.Add(PrevHitUVLoc);
		}
	}
	
}

void AWhiteboard::LoadBoard()
{
	ClearBoard();
	FVector2D PrevDraw{ 0,0 };
	DrawArray = DataSaveLoader->LoadVec2Arr();
	bool LoadFirst = true;
	for (auto point : DrawArray) {
		if (point.X > 0) {
			if (LoadFirst) {
				PrevDraw = point;
				LoadFirst = false;
			}
			else {
				UCanvas* Canvas;
				FVector2D Size;
				FDrawToRenderTargetContext Context;
				UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);
				if (Canvas) {
					Canvas->K2_DrawLine(PrevDraw, point, BrushThickness, DrawColor);
				}
				UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
				PrevDraw = point;
			}
		}
		else {
			LoadFirst = true;
		}
	}
}

void AWhiteboard::ClearBoard()
{
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, InitialColor);
	DrawArray.Empty();
}

void AWhiteboard::SaveBoard()
{
	DataSaveLoader->SaveVec2Arr(DrawArray);
}

