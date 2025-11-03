// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentObjGenOn.h"
#include "Objectives/ObjectiveActorTurnOnAllGenerator.h"

void UDelegateBindingComponentObjGenOn::BindDelegateFromBP()
{
}

void UDelegateBindingComponentObjGenOn::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AObjectiveActorTurnOnAllGenerator* ObjectiveActorTurnOnAllGeneratorOwner =
			Cast<AObjectiveActorTurnOnAllGenerator>(GetOwner()))
		{
			GameStateEventManager->OnGeneratorInteractDelegate.AddUniqueDynamic(
				ObjectiveActorTurnOnAllGeneratorOwner,
				&AObjectiveActorTurnOnAllGenerator::BroadcastUIUpdateEvent);
			
			GameStateEventManager->OnGeneratorInteractDelegate.AddUniqueDynamic(
				ObjectiveActorTurnOnAllGeneratorOwner,
				&AObjectiveActorTurnOnAllGenerator::IfCompleteTriggerSubObjectivesAndWave);
			
			GameStateEventManager->FOnForceFieldDeathDelegate.AddUniqueDynamic(
				ObjectiveActorTurnOnAllGeneratorOwner,
				&AObjectiveActorTurnOnAllGenerator::BroadcastCompletionReverseredEvent);
			
			GameStateEventManager->OnGeneratorTimerManagerTimerFinishedDelegate.AddUniqueDynamic(
				ObjectiveActorTurnOnAllGeneratorOwner,
				&AObjectiveActorTurnOnAllGenerator::OnTimerFinishedSetToTrue);
		}
	}
}
