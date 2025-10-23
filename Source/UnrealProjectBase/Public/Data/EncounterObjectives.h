// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EncounterObjectives.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UEncounterObjectives : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Objectives")
	TArray<FString> EncounterObjectives;
};
