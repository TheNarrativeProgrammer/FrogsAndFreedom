// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentObjSpawn.h"
#include "Objectives/ObjectiveActorSpawnersActive.h"


void UDelegateBindingComponentObjSpawn::BindDelegateFromBP_Implementation()
{
	Super::BindDelegateFromBP_Implementation();
}

void UDelegateBindingComponentObjSpawn::BindDelegates()
{
	Super::BindDelegates();

	if (GameStateEventManager)
	{
		if (AObjectiveActorSpawnersActive* ObjectiveActorSpawnersActiveOwner = Cast<AObjectiveActorSpawnersActive>(GetOwner()))
		{
			GameStateEventManager->OnActorSpawnerBaseMachineDeathDelegate.AddUniqueDynamic(ObjectiveActorSpawnersActiveOwner, &AObjectiveActorSpawnersActive::BroadcastUIUpdateEvent);
			GameStateEventManager->OnActorSpawnerBaseMachineDeathDelegate.AddUniqueDynamic(ObjectiveActorSpawnersActiveOwner, &AObjectiveActorSpawnersActive::IfCompleteTriggerSubObjectivesAndWave);
		}
	}
	
}
