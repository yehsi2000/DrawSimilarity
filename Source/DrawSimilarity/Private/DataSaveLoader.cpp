#include "DataSaveLoader.h"
#include "Misc/FileHelper.h"

UDataSaveLoader::UDataSaveLoader()
{
	PrimaryComponentTick.bCanEverTick = false;
	SaveName = "Base";
}

void UDataSaveLoader::SaveVec2Arr(const TArray<FVector2D> Vec2Data)
{
	if (SaveName.Len() < 1) return;
	SavePath = FPaths::ProjectContentDir().Append(TEXT("/Data/")).Append(SaveName);

	TArray<FString> StrArr;
	for (auto vec : Vec2Data) {
		TMap<FString, FStringFormatArg> FormatArg;
		FormatArg.Emplace(TEXT("X"), vec.X);
		FormatArg.Emplace(TEXT("Y"), vec.Y);
		StrArr.Add(FString::Format(TEXT("{X},{Y}"), FormatArg));
	}

	FFileHelper::SaveStringArrayToFile(StrArr,*SavePath);

}

TArray<FVector2D> UDataSaveLoader::LoadVec2Arr()
{
	

	TArray<FVector2D> RetArr;
	if (SaveName.Len() < 1) return RetArr;
	SavePath = FPaths::ProjectContentDir().Append(TEXT("/Data/")).Append(SaveName);

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*SavePath))
	{
		return RetArr;
	}

	TArray<FString> StrArr;
	FFileHelper::LoadFileToStringArray(StrArr,*SavePath);
	for (FString VecStr : StrArr)
	{
		TArray<FString> VecStrArr;
		VecStr.ParseIntoArray(VecStrArr,TEXT(","));
		RetArr.Emplace(FCString::Atof(*VecStrArr[0].TrimQuotes()), FCString::Atof(*VecStrArr[1].TrimQuotes()));
	}

	return RetArr;
}
