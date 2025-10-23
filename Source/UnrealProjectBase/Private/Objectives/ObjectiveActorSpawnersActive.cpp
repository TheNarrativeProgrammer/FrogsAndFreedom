// Copyright Peasant Pals 2025


#include "Objectives/ObjectiveActorSpawnersActive.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BindComponents/DelegateBindingComponentObjSpawn.h"
#include "Environment/ActorSpawner/ActorSpawnerBaseFrogMachine.h"

AObjectiveActorSpawnersActive::AObjectiveActorSpawnersActive()
{
	DelegateBindingComponentObjSpawn = CreateDefaultSubobject<UDelegateBindingComponentObjSpawn>(TEXT("DelegateBindingComponentObjSpawn"));
}

bool AObjectiveActorSpawnersActive::IsObjectiveCompleted_Implementation() const
{
	if (_mIsAutoCompleteObjective == true)
	{
		return true;
	}
	for (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine : _msActorSpawnerMachinesInLevel)
	{
		if (IsValid(ActorSpawnerBaseFrogMachine) == true)
		{
			if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == true)
			{
				return false;
			}
		}
	}
	
	return Super::IsObjectiveCompleted_Implementation();
}

FText AObjectiveActorSpawnersActive::GetObjectiveDisplayText_Implementation() const
{
	int DeacctivedSpawners = 0;
	for (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine : _msActorSpawnerMachinesInLevel)
	{
		if (IsValid(ActorSpawnerBaseFrogMachine) == true)
		{
			if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == false)
			{
				DeacctivedSpawners++;
			}
		}
	}

#define LOCTEXT_NAMESPACE "ObjectiveActorSpawnersActive"
	FFormatNamedArguments Args;
	Args.Add(TEXT("DisplayTextkey"), _mObjectiveDisplayName);
	Args.Add(TEXT("DectivedSpawnersKey"), FText::AsNumber(DeacctivedSpawners));
	Args.Add(TEXT("TotalSpawnersKey"), FText::AsNumber(_msActorSpawnerMachinesInLevel.Num()));
	const FText ActiveSpawnersFormat = LOCTEXT("ActiveSpawnersProgressFormat", "{DisplayTextkey} : {DectivedSpawnersKey} / {TotalSpawnersKey}");

	FText OutputText = FText::Format(ActiveSpawnersFormat, Args);
	return OutputText;
#undef LOCTEXT_NAMESPACE
}

void AObjectiveActorSpawnersActive::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	_mHasObjectiveStarted = true;

	if (AGameStateEventManagerBase* GameStateEventManager = Cast<AGameStateEventManagerBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
	}
}

void AObjectiveActorSpawnersActive::CompleteObjectiveBroadcast_Implementation()
{
	Super::CompleteObjectiveBroadcast_Implementation();
}

void AObjectiveActorSpawnersActive::BroadcastUIUpdateEvent()
{
	DelegateBindingComponentObjSpawn->GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
}
