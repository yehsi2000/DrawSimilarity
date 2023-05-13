#include "SimilarityEvalComponent.h"

USimilarityEvalComponent::USimilarityEvalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BoardResolution = 512;
}


float USimilarityEvalComponent::HausdorffDist(const TArray<FVector2D>& ShapeX, const TArray<FVector2D>& ShapeY) const
{
	UE_LOG(LogTemp, Warning, TEXT("Hausdorff ShapeX Length : %d, ShapeY Length : %d"), ShapeX.Num(), ShapeY.Num());

	float maxDfromX = 0;
	for (auto pointX : ShapeX) {
		if(pointX.X<0) continue;
		float minDXtoY = 1.42 * BoardResolution; //sqrt(2) * BoardSize = Max Distance
		for (auto pointY : ShapeY) {
			if (pointY.X < 0) continue;
			float dist = FVector2D::Distance(pointX, pointY);
			minDXtoY = FMath::Min(dist, minDXtoY);
		}
		maxDfromX = FMath::Max(maxDfromX,minDXtoY);
	}

	float maxDfromY = 0;
	for (auto pointY : ShapeY) {
		if (pointY.X < 0) continue;
		float minDYtoX = 1.42 * BoardResolution; //sqrt(2) * BoardSize = Max Distance
		for (auto pointX : ShapeX) {
			if (pointX.X < 0) continue;
			float dist = FVector2D::Distance(pointX, pointY);
			minDYtoX = FMath::Min(dist, minDYtoX);
		}
		maxDfromY = FMath::Max(maxDfromY,minDYtoX);
	}

	return FMath::Max(maxDfromX,maxDfromY);
}

float USimilarityEvalComponent::AverageHausdorffDist(const TArray<FVector2D>& ShapeX, const TArray<FVector2D>& ShapeY) const
{
	UE_LOG(LogTemp, Warning, TEXT("Average Hausdorff ShapeX Length : %d, ShapeY Length : %d"), ShapeX.Num(), ShapeY.Num());

	float totalDfromX = 0;
	for (auto pointX : ShapeX) {
		if (pointX.X < 0) continue;
		float minDXtoY = 1.42 * BoardResolution; //sqrt(2) * BoardSize = Max Distance
		for (auto pointY : ShapeY) {
			if (pointY.X < 0) continue;
			float dist = FVector2D::Distance(pointX, pointY);
			minDXtoY = FMath::Min(dist, minDXtoY);
		}
		totalDfromX += minDXtoY;
	}

	float totalDfromY = 0;
	for (auto pointY : ShapeY) {
		if (pointY.X < 0) continue;
		float minDYtoX = 1.42 * BoardResolution; //sqrt(2) * BoardSize = Max Distance
		for (auto pointX : ShapeX) {
			if (pointX.X < 0) continue;
			float dist = FVector2D::Distance(pointX, pointY);
			minDYtoX = FMath::Min(dist, minDYtoX);
		}
		totalDfromY += minDYtoX;
	}

	return totalDfromX/ ShapeX.Num() + totalDfromY/ ShapeY.Num();
}

TArray<FVector2D> USimilarityEvalComponent::CenteredShape(const TArray<FVector2D>& Shape) const
{
	UE_LOG(LogTemp, Warning, TEXT("Center Shape Length : %d"),Shape.Num());

	FVector2D ShapeSum(0,0);

	for (auto point : Shape) {
		if(point.X<0) continue;
		ShapeSum += point;
	}

	FVector2D AvgPoint = ShapeSum / Shape.Num();

	TArray<FVector2D> NewShape;

	for (auto& point : Shape) {
		if (point.X < 0) continue;
		NewShape.Add(point - AvgPoint);
	}

	return NewShape;
}