// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EncounterObjective.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UEncounterObjective : public UDataAsset
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Objectives")
	TArray<FString> EncounterObjectives;
	
};
