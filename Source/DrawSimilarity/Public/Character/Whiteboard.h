// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Whiteboard.generated.h"

UCLASS()
class DRAWSIMILARITY_API AWhiteboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWhiteboard();

protected:
	// Network
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	class UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WhiteBoard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Background;

	UPROPERTY(EditDefaultsOnly)
	class UDataSaveLoader* DataSaveLoader;

	UPROPERTY(EditDefaultsOnly)
	float BrushThickness;

	UPROPERTY(BlueprintReadWrite)
	FColor InitialColor;

	UPROPERTY(BlueprintReadWrite)
	FColor DrawColor;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* Material;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_DrawArray)
	TArray<FVector2D> DrawArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D BoardResolution = {512,512};

	FVector2D PrevHitUVLoc;

	UFUNCTION()
	void OnRep_DrawArray();
public:	

	UFUNCTION(BlueprintCallable)
	void DrawLine(FHitResult hit, bool inithit);

	UFUNCTION(BlueprintCallable)
	void LoadBoard();

	UFUNCTION(BlueprintCallable)
	void ClearBoard();

	UFUNCTION(BlueprintCallable)
	void SaveBoard();
};
