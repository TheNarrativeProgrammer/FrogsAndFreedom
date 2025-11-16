// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FMODEvent.h"
#include "GameplayTagContainer.h"
#include "DialogueDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDialogueTextData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Dialogue Text")
	FText DialogueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Dialogue Text")
	float TimeToDisplayText = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta=(Categories="DialogueCharacters"), Category="Dialogue Text")
	FGameplayTag CharacterSpeakingGameplayTag;
};

USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag RowLoopUpGameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UFMODEvent* FmodAudioEvent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Dialogue"))
	TArray<FDialogueTextData> DialogueTextData;
};

UCLASS()
class UNREALPROJECTBASE_API UDialogueDataTable : public UDataTable
{
	GENERATED_BODY()
};
