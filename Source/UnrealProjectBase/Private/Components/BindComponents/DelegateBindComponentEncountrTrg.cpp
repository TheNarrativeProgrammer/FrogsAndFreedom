// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentEncountrTrg.h"
#include "Environment/ActorSpawner/EncounterTrigger.h"
#include "Environment/Doors/LevelDoorBase.h"

void UDelegateBindComponentEncountrTrg::BindDelegateFromBP()
{
}

void UDelegateBindComponentEncountrTrg::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AEncounterTrigger* EncounterTriggerOwner = Cast<AEncounterTrigger>(GetOwner()))
		{
			GameStateEventManager->OnSingleObectiveCompleteDelegate.AddUniqueDynamic(
				EncounterTriggerOwner, &AEncounterTrigger::IfAllObjectivesCompeleteThenBroadcast);
		}
	}
}
