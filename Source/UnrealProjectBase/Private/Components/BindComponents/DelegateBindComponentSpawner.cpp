// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentSpawner.h"
#include "Environment/ActorSpawner/ActorSpawnerBaseFrogMachine.h"

void UDelegateBindComponentSpawner::BindDelegateFromBP()
{
	Super::BindDelegateFromBP();
}

void UDelegateBindComponentSpawner::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachineOwner = Cast<AActorSpawnerBaseFrogMachine>(GetOwner()))
		{
			GameStateEventManager->OnCharacterBaseFrogTankDeathIDDelegate.AddUniqueDynamic(
				ActorSpawnerBaseFrogMachineOwner, &AActorSpawnerBaseFrogMachine::OnTankDeathCheckIDMatches);
			GameStateEventManager->OnCharacterBaseFrogTankSpawnedLocationDelegate.AddUniqueDynamic(
				ActorSpawnerBaseFrogMachineOwner, &AActorSpawnerBaseFrogMachine::SetTankLocation);
			GameStateEventManager->OnEncounterStartedDelegate.AddUniqueDynamic(
				ActorSpawnerBaseFrogMachineOwner, &AActorSpawnerBaseFrogMachine::OnEncounterStartBroadcastActorInitializationDelegates);
		}
	}
}
