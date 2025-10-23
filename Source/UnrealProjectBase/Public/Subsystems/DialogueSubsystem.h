// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FMODEvent.h"
#include "Data/DialogueDataTable.h"

#include "DialogueSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueTextChangeSignature, FText, DialogText, FGameplayTag, CharacterSpeakingTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueQueEmptySignature);



UCLASS()
class UNREALPROJECTBASE_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	TQueue<FDialogueData> DialogueQueue;
	TQueue<TArray<FDialogueTextData>> DialogueTextQueue;

	UPROPERTY()
	TArray<FDialogueTextData> CurrentDialogueTextDataOfRow;

	UPROPERTY()
	FDialogueData CurrentDialogueOfRow;
	
	UPROPERTY()
	bool _mIsDialogueCurrentlyPlaying = false;
	
	UPROPERTY(BlueprintReadWrite)
	UDataTable* DialogueTable;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerHandle;

	UFUNCTION()
	void ConvertTagToString(FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable)
	TArray<FDialogueTextData> GetDiaglogueTextOfTableRow (FName GameplayTagAsName);

	UFUNCTION(BlueprintCallable)
	UFMODEvent* GetDiaglogueFmodEventOfTableRow (FName GameplayTagAsName);
	
	UFUNCTION(BlueprintCallable)
	void IterateDialogueQue ();

	UFUNCTION(BlueprintCallable)
	void IterateDialogueText();

	UFUNCTION(BlueprintCallable)
	void OnDialogueFinishedIterate();

	UFUNCTION(BlueprintCallable)
	void PlayDialogue();

	
public:
	UFUNCTION(BlueprintCallable)
	void SetDialogueTable (UDataTable* InDialogueTable);
	
	UFUNCTION(BlueprintCallable)
	void AddDialogueTextDataToQue (FName GameplayTagAsName);

	UFUNCTION(BlueprintCallable)
	void TriggerDialogueIfNonPlaying ();

	UFUNCTION(BlueprintCallable)
	void OnGameOverClearTimer();
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Dialogue")
	FOnDialogueTextChangeSignature OnDialogueTextChangeDelegate;
	
};
