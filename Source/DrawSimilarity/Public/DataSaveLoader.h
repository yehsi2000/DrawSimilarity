// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataSaveLoader.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAWSIMILARITY_API UDataSaveLoader : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDataSaveLoader();
	
	UFUNCTION(BlueprintCallable)
	void SaveVec2Arr(const TArray<FVector2D> Vec2Data);

	UFUNCTION(BlueprintPure)
	TArray<FVector2D> LoadVec2Arr();

	UPROPERTY(EditAnywhere)
	FString SaveName;
private:
	FString SavePath;
};
