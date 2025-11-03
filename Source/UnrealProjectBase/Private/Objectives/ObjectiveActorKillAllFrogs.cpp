// Copyright Peasant Pals 2025


#include "Objectives/ObjectiveActorKillAllFrogs.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameStateEventManagerBase.h"

AObjectiveActorKillAllFrogs::AObjectiveActorKillAllFrogs()
{
	DelegateBindComponentObjKillAll = CreateDefaultSubobject<UDelegateBindComponentObjKillAll>(TEXT("Delegate Bind Component"));
}

void AObjectiveActorKillAllFrogs::BeginPlay()
{
	Super::BeginPlay();
}

bool AObjectiveActorKillAllFrogs::IsObjectiveCompleted_Implementation() const
{
	if (_mIsAutoCompleteObjective == true)
	{
		return true;
	}
	if (_mAreAllFrogsDead == true && Super::IsObjectiveCompleted_Implementation() == true)
	{
		return true;
	}
	return false;
}

void AObjectiveActorKillAllFrogs::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	// if (AGameStateEventManagerBase* GameStateEventManager = Cast<AGameStateEventManagerBase>(UGameplayStatics::GetGameState(GetWorld())))
	// {
	// 	GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
	// }
}

void AObjectiveActorKillAllFrogs::CompleteObjectiveBroadcast_Implementation()
{
	Super::CompleteObjectiveBroadcast_Implementation();
}

FText AObjectiveActorKillAllFrogs::GetObjectiveDisplayText_Implementation() const
{
#define LOCTEXT_NAMESPACE "ObjectiveActorEnemyKillCount"
	FFormatNamedArguments Args;
	Args.Add(TEXT("DisplayTextkey"), _mObjectiveDisplayName);
	Args.Add(TEXT("TotalEmeniesKilledKey"), FText::AsNumber(_mCountEnemiesKilled));
	Args.Add(TEXT("TotalEnemiesAliveKey"), FText::AsNumber(_mCountTotalEnemiesSpawned));
	const FText EnemyKillCountFormat = LOCTEXT("ObjectiveActorEnemyKillCount", "{DisplayTextkey} : {TotalEmeniesKilledKey} / {TotalEnemiesAliveKey}");

	FText OutputText = FText::Format(EnemyKillCountFormat, Args);
	return OutputText;
#undef LOCTEXT_NAMESPACE
}



void AObjectiveActorKillAllFrogs::SetAreAllFrogsDead(bool InAreAllFrogsDead)
{
	_mAreAllFrogsDead = InAreAllFrogsDead;
}

void AObjectiveActorKillAllFrogs::IncreaseCountTotalEnemiesSpawned()
{
	_mCountTotalEnemiesSpawned++;
	if (IsObjectiveStarted() == true)
	{
		if (DelegateBindComponentObjKillAll)
		{
			DelegateBindComponentObjKillAll->GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
		}

		if (_mCountEnemiesKilled < (_mCountTotalEnemiesSpawned * 1.2) && _mAreAllFrogsDead == true)
		{
			SetAreAllFrogsDead(false);
			SetIsFreed(false);
			SetIsCompletionTextVisible(true);
			if (DelegateBindComponentObjKillAll)
			{
				DelegateBindComponentObjKillAll->GameStateEventManager->
					OnActorObjectiveCompletionReversedUpdateUIDelegate.Broadcast(this);
				
			}
		}
	}
}

void AObjectiveActorKillAllFrogs::IncreaseCountEnemiesKilled()
{
	_mCountEnemiesKilled++;
	
	if (_mCountEnemiesKilled >= _mCountTotalEnemiesSpawned * 0.9 && IsObjectiveCompleted() == false && IsObjectiveStarted() == true)
	{
		SetAreAllFrogsDead(true);
		IfCompleteTriggerSubObjectivesAndWave();
	}
	else if (IsObjectiveStarted() == true)
	{
		if (DelegateBindComponentObjKillAll)
		{
			DelegateBindComponentObjKillAll->GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
		}
	}
}


