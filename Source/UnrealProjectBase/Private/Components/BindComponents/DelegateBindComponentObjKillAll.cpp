// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentObjKillAll.h"

#include "Objectives/ObjectiveActorKillAllFrogs.h"

void UDelegateBindComponentObjKillAll::BindDelegateFromBP_Implementation()
{
	Super::BindDelegateFromBP_Implementation();
}

void UDelegateBindComponentObjKillAll::BindDelegates()
{
	if (GameStateEventManager)
	{
		if (AObjectiveActorKillAllFrogs* ObjectiveActorKillAllFrogsOwner = Cast<AObjectiveActorKillAllFrogs>(GetOwner()))
		{
			GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.AddUniqueDynamic(ObjectiveActorKillAllFrogsOwner, &AObjectiveActorKillAllFrogs::IncreaseCountTotalEnemiesSpawned);
			GameStateEventManager->OnCharacterBaseFrogDeathDelegate.AddUniqueDynamic(ObjectiveActorKillAllFrogsOwner, &AObjectiveActorKillAllFrogs::IncreaseCountEnemiesKilled);
			
		}
	}
	
}
