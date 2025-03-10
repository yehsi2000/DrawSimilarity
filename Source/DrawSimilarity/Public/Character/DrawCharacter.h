// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DrawCharacter.generated.h"

UCLASS()
class DRAWSIMILARITY_API ADrawCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADrawCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UFUNCTION(Server, Reliable)
	void ServerDraw(FVector CameraLocation, FVector ForwardVector, bool IsInitialTouch);

	UFUNCTION(BlueprintCallable)
	void Draw(FVector CameraLocation, FVector ForwardVector, bool IsInitialTouch);

	APlayerCameraManager* PlayerCameraManager;

	UPROPERTY(BlueprintReadWrite)
	float TraceDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDrawing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInitialTouch = false;
};
