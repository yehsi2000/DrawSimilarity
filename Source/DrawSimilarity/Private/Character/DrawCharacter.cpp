

#include "Character/DrawCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Character/Whiteboard.h"

ADrawCharacter::ADrawCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ADrawCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsInitialTouch = false;
	PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	TraceDistance = 500.f;
}


void ADrawCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDrawing)
	{
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FVector ForwardVector = PlayerCameraManager->GetActorForwardVector();
		if (!HasAuthority()) {
			ServerDraw(CameraLocation, ForwardVector, bIsInitialTouch);
		}

		Draw(CameraLocation, ForwardVector, bIsInitialTouch);
		
	}
}

void ADrawCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADrawCharacter::ServerDraw_Implementation(FVector CameraLocation, FVector ForwardVector, bool IsInitialTouch)
{
	Draw(CameraLocation, ForwardVector, IsInitialTouch);
}

void ADrawCharacter::Draw(FVector CameraLocation, FVector ForwardVector, bool IsInitialTouch)
{
	//FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	//FVector ForwardVector = PlayerCameraManager->GetActorForwardVector();
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(this, CameraLocation, CameraLocation + ForwardVector * TraceDistance, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true);
	if (HitResult.GetActor()) {
		AWhiteboard* drawboard = Cast<AWhiteboard>(HitResult.GetActor());
		if (drawboard)
		{
			drawboard->DrawLine(HitResult, IsInitialTouch);
			bIsInitialTouch = false;
		}
	}
	
}

