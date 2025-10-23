// Copyright Peasant Pals 2025


#include "Subsystems/DialogueSubsystem.h"
#include "GameplayTags.h"
#include "GameFramework/Character.h"
#include "FMODBlueprintStatics.h"
#include "ShaderPrintParameters.h"
#include "Core/GameStateEventManagerBase.h"
#include "Kismet/GameplayStatics.h"

void UDialogueSubsystem::ConvertTagToString(FGameplayTag GameplayTag)
{
	FName RowName = GameplayTag.GetTagName();
}

TArray<FDialogueTextData>  UDialogueSubsystem::GetDiaglogueTextOfTableRow(FName GameplayTagAsName)
{
	FString LookUpContext = "Dialogue Text Query";
	FDialogueData* DialogueDataForRow = DialogueTable->FindRow<FDialogueData>(GameplayTagAsName, LookUpContext);

	if (DialogueDataForRow)
	{
		return DialogueDataForRow->DialogueTextData;
	}
	//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: GetDiaglogueTextOfTableRow was passed an invalid Gameplaytag for text."))
	return TArray<FDialogueTextData>();
}

UFMODEvent* UDialogueSubsystem::GetDiaglogueFmodEventOfTableRow(FName GameplayTagAsName)
{
	FString LookUpContext = "Dialogue Audio Query";
	FDialogueData* DialogueDataForRow = DialogueTable->FindRow<FDialogueData>(GameplayTagAsName, LookUpContext);
	if (DialogueDataForRow)
	{
		return DialogueDataForRow->FmodAudioEvent;
	}
	//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: GetDiaglogueFmodEventOfTableRow was passed an invalid Gameplaytag for audio"));
	return nullptr;
}

void UDialogueSubsystem::AddDialogueTextDataToQue(FName GameplayTagAsName)
{
	FString LookUpContext = "Dialogue Query add to que";
	 
	//TArray<FDialogueTextData> DialogueTextData = GetDiaglogueTextOfTableRow(GameplayTagAsName);

	
	FDialogueData* DialogueData = DialogueTable->FindRow<FDialogueData>(GameplayTagAsName, LookUpContext);

	if (DialogueData)
	{
		DialogueQueue.Enqueue(*DialogueData);
	}
	
	
	//DialogueTextQueue.Enqueue(DialogueTextData);
}

void UDialogueSubsystem::TriggerDialogueIfNonPlaying()
{
	if (_mIsDialogueCurrentlyPlaying == false)
	{
		IterateDialogueQue();
	}
}

void UDialogueSubsystem::OnGameOverClearTimer()
{
	_mIsDialogueCurrentlyPlaying = false;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UDialogueSubsystem::IterateDialogueQue()
{
	if (DialogueQueue.IsEmpty() == false)
	{
		if (_mIsDialogueCurrentlyPlaying == false)
		{
			bool IsSuccessfulRow = DialogueQueue.Dequeue(CurrentDialogueOfRow);

			if (IsSuccessfulRow)
			{
				CurrentDialogueTextDataOfRow = CurrentDialogueOfRow.DialogueTextData;
			
				_mIsDialogueCurrentlyPlaying = true;
				PlayDialogue();
				IterateDialogueText();
			}
		}
		
	}
	else
	{
		if (AGameStateEventManagerBase* GameStateEventManagerBase = Cast<AGameStateEventManagerBase>(GetWorld()->GetGameState()))
		{
			GameStateEventManagerBase->OnDialogueFinishedDelegate.Broadcast();
		}
	}
	
	
}

void UDialogueSubsystem::IterateDialogueText()
{

	if (CurrentDialogueTextDataOfRow.IsValidIndex(0))
	{
		OnDialogueTextChangeDelegate.Broadcast(CurrentDialogueTextDataOfRow[0].DialogueText, CurrentDialogueTextDataOfRow[0].CharacterSpeakingGameplayTag);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDialogueSubsystem::IterateDialogueText, CurrentDialogueTextDataOfRow[0].TimeToDisplayText, false);
		}
		CurrentDialogueTextDataOfRow.RemoveAt(0);
	}
	else if (CurrentDialogueTextDataOfRow.IsEmpty() && CurrentDialogueTextDataOfRow.IsValidIndex(0) == false)
	{
		OnDialogueFinishedIterate();
	}
}

void UDialogueSubsystem::OnDialogueFinishedIterate()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	if (CurrentDialogueTextDataOfRow.IsEmpty() == true || CurrentDialogueTextDataOfRow.Num() == 0)
	{
		CurrentDialogueTextDataOfRow.Empty();
		if (DialogueQueue.IsEmpty() == false)
		{
			_mIsDialogueCurrentlyPlaying = false;
			IterateDialogueQue();
		}
		else
		{
			_mIsDialogueCurrentlyPlaying = false;
		}
	}
}

void UDialogueSubsystem::PlayDialogue()
{
	// FVector SoundLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	// UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), CurrentDialogueOfRow.FmodAudioEvent, FTransform(SoundLocation), true);
	
	if (CurrentDialogueOfRow.FmodAudioEvent)
	{
		if (USceneComponent* SoundLocationPlayerRoot = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetRootComponent())
		{
			UFMODAudioComponent* AudioComponent = UFMODBlueprintStatics::PlayEventAttached(
			CurrentDialogueOfRow.FmodAudioEvent,
			SoundLocationPlayerRoot,
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::Type::KeepRelativeOffset,
			true,
			true,
			false
			);

			if (AudioComponent)
			{
				AudioComponent->Release();
			}
		}
	
		
	}
	
}

void UDialogueSubsystem::SetDialogueTable(UDataTable* InDialogueTable)
{
	DialogueTable = InDialogueTable;
}




