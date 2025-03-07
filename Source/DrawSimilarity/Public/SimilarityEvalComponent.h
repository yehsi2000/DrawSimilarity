// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimilarityEvalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAWSIMILARITY_API USimilarityEvalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimilarityEvalComponent();
	
	UFUNCTION(BlueprintPure, Category = "Similarity")
	float HausdorffDist(const TArray<FVector2D>& ShapeX, const TArray<FVector2D>& ShapeY) const;

	UFUNCTION(BlueprintPure, Category = "Similarity")
	float AverageHausdorffDist(const TArray<FVector2D>& ShapeX, const TArray<FVector2D>& ShapeY) const;

	UFUNCTION(BlueprintPure, Category = "Similarity")
	TArray<FVector2D> CenteredShape(const TArray<FVector2D>& Shape) const;

	UPROPERTY(EditAnywhere,Category = "Similarity")
	int BoardResolution;

};
