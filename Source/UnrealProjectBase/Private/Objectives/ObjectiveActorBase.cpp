// Copyright Peasant Pals 2025


#include "Objectives/ObjectiveActorBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EncounterManagerComponent.h"
#include "Core/GameStateEventManagerBase.h"
#include "Environment/ActorSpawner/EncounterTrigger.h"


// Sets default values
AObjectiveActorBase::AObjectiveActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectiveActorBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AObjectiveActorBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = _mWaveToTriggerOnObjectiveComplete;
}




void AObjectiveActorBase::SetIsAutoCompleteObjective(bool InIsAutoCompleteObjective)
{
	_mIsAutoCompleteObjective = InIsAutoCompleteObjective;
}

void AObjectiveActorBase::IncreaseCountOfPreObjectivesCompleted()
{
	_mCountNumberOfPreObjectivesCompleted++;
}

void AObjectiveActorBase::SpawnWave1IfStartIsObjectiveDependent(
	UEncounterManagerComponent* EncounterManagerComponent,
	const FGameplayTagContainer& InWaveToTriggerOnObjectiveComplete) const
{
	if (!EncounterManagerComponent) return;
	if (!EncounterManagerComponent->CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	if(EncounterManagerComponent->CurrentActiveWave.MatchesTagExact(
				FGameplayTag::RequestGameplayTag(FName("Wave.1"), false)))
	{
		if (InWaveToTriggerOnObjectiveComplete.HasTag(
			FGameplayTag::RequestGameplayTag(FName("Wave.1"))))
		{
			
			FEncounterWaveData EncounterWaveData = EncounterManagerComponent->
				CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(
				FGameplayTag::RequestGameplayTag(FName("Wave.1"), false));

			if (EncounterWaveData.WaveTrigger == EWaveTrigger::Objective)
			{
				EncounterManagerComponent->OnObjectiveCompleteStartSpawningWave(
					FGameplayTag::RequestGameplayTag(FName("Wave.1")));
			}
		}
	}
}

bool AObjectiveActorBase::IsNextWaveInQue(
	UEncounterManagerComponent* EncounterManagerComponent,
	const FGameplayTag InWaveToTriggerOnObjectiveComplete) const
{
	if (InWaveToTriggerOnObjectiveComplete.IsValid())
	{
		EncounterManagerComponent->AddWaveToObjectiveCompletionMap(
				InWaveToTriggerOnObjectiveComplete);
		
		if (EncounterManagerComponent->NextWaveInQue == InWaveToTriggerOnObjectiveComplete
			&& EncounterManagerComponent->GetIsSpawning() == false)
		{
			EncounterManagerComponent->SetActiveWave(InWaveToTriggerOnObjectiveComplete);
			
			EncounterManagerComponent->OnObjectiveCompleteStartSpawningWave(
				InWaveToTriggerOnObjectiveComplete);
			return true;
		}
	}
	return false;
}




void AObjectiveActorBase::SetIsFreed_Implementation(bool InIsFreed)
{
	_mIsFreed = InIsFreed;
}

void AObjectiveActorBase::StopObjective_Implementation()
{
	_mHasObjectiveStarted = false;
}


void AObjectiveActorBase::CompleteObjectiveBroadcast_Implementation()
{
	UWorld* World = GetWorld();
	if (!World) return;
	SetIsCompletionTextVisible(true);

	if (SubObjectives.IsEmpty() == true)
	{
		if (AGameStateEventManagerBase* GameStateEventManager =
			Cast<AGameStateEventManagerBase>(UGameplayStatics::GetGameState(World)))
		{
			if (GameStateEventManager)
			{
				GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
			}
		}
	}
	if (World)
	{
		bool isTimerActive = World->GetTimerManager().IsTimerActive(TimerHandle);
		if (isTimerActive == false)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
				this, &AObjectiveActorBase::CompleteObjective);
			
			World->GetTimerManager().SetTimer(TimerHandle,
				TimerDelegate, mTimeDisplayCompletionText, false);
		}
	}
}

bool AObjectiveActorBase::IsObjectiveStarted() const
{
	return _mHasObjectiveStarted;
}

bool AObjectiveActorBase::IsObjectivedFreed() const
{
	return _mIsFreed;
}

bool AObjectiveActorBase::IsObjectiveCompleted_Implementation() const
{
	return _mHasObjectiveStarted;
}

void AObjectiveActorBase::TriggerWave(const FGameplayTagContainer& InWaveToTriggerOnObjectiveComplete) const
{
	if (InWaveToTriggerOnObjectiveComplete.IsEmpty())
	{
		return;
	}
	if (AGameModeBase* Gamemode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		if (UEncounterManagerComponent* EncounterManagerComponent = Cast<UEncounterManagerComponent>(
			Gamemode->GetComponentByClass(UEncounterManagerComponent::StaticClass())))
		{
			if (InWaveToTriggerOnObjectiveComplete.IsEmpty()) return;
			
			SpawnWave1IfStartIsObjectiveDependent(EncounterManagerComponent, InWaveToTriggerOnObjectiveComplete);

			bool ShouldNotCheckNextTag = false;
			for (auto WaveGameplayTag : InWaveToTriggerOnObjectiveComplete)
			{
				if (ShouldNotCheckNextTag == false)
				{
					ShouldNotCheckNextTag = IsNextWaveInQue(EncounterManagerComponent, WaveGameplayTag);
				}
				else
				{
					EncounterManagerComponent->AddWaveToObjectiveCompletionMap(WaveGameplayTag);
				}
			}
		}
	}
}

bool AObjectiveActorBase::IsCompletionTextVisible_Implementation() const
{
	return _mIsCompletionTextVisible;
}

void AObjectiveActorBase::CompleteObjective()
{
	SetIsCompletionTextVisible(false);
	UWorld* World = GetWorld();
	if (!World) return;
	if (AGameStateEventManagerBase* GameStateEventManager = Cast<AGameStateEventManagerBase>(UGameplayStatics::GetGameState(World)))
	{
		if (GameStateEventManager)
		{
			GameStateEventManager->OnSingleObectiveCompleteDelegate.Broadcast();
		}
	}
}


void AObjectiveActorBase::SetIsCompletionTextVisible_Implementation(bool InIsCompletionTextVisilbe)
{
	_mIsCompletionTextVisible = InIsCompletionTextVisilbe;
}

void AObjectiveActorBase::IfCompleteTriggerSubObjectivesAndWave()
{
	if (IsObjectiveCompleted_Implementation())
	{
		TriggerWave(_mWaveToTriggerOnObjectiveComplete);
		CompleteObjectiveBroadcast();
		StartSubObjectives();
	}
}




FText AObjectiveActorBase::GetCompletionDisplayText_Implementation() const
{
	return _mCompletionDisplayName;
}

void AObjectiveActorBase::StartSubObjectives() const
{
	if (SubObjectives.IsEmpty() == true) return;
	for (AObjectiveActorBase* ObjectiveBase : SubObjectives)
	{
		if (ObjectiveBase != nullptr && ObjectiveBase != this)
		{
			if (ObjectiveBase->_mReuqiredNumberOfPreObjectivesCompletedToStart <= 1)
			{
				ObjectiveBase->StartObjective();
			}
			else
			{
				ObjectiveBase->IncreaseCountOfPreObjectivesCompleted();
				if (ObjectiveBase->_mCountNumberOfPreObjectivesCompleted >=
					ObjectiveBase->_mReuqiredNumberOfPreObjectivesCompletedToStart)
				{
					ObjectiveBase->StartObjective();
				}
			}
		}
	}
}

void AObjectiveActorBase::StopSubObjectives() const
{
	if (SubObjectives.IsEmpty() == true) return;
	for (AObjectiveActorBase* ObjectiveBase : SubObjectives)
	{
		if (ObjectiveBase != nullptr && ObjectiveBase != this)
		{
			ObjectiveBase->StopObjective();
		}
	}
}

void AObjectiveActorBase::StartObjective_Implementation()
{
	_mHasObjectiveStarted = true;
	SetIsFreed(false);
	SetIsCompletionTextVisible(true);
}

FText AObjectiveActorBase::GetObjectiveDisplayText_Implementation() const
{
	return _mObjectiveDisplayName;
}

// Called when the game starts or when spawned
void AObjectiveActorBase::BeginPlay()
{
	Super::BeginPlay();
	SetIsFreed(false);
	SetIsCompletionTextVisible(true);
	//ObjectiveEventCompletedToggleCompletionTextDelegate.AddUniqueDynamic(this, &AObjectiveActorBase::SetIsCompletionTextVisible);
	
}

// Called every frame
void AObjectiveActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

