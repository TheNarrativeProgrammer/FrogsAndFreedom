// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentSlimeManger.h"
#include "Environment/ActorSpawner/SpawningSlimeManager.h"

void UDelegateBindComponentSlimeManger::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (ASpawningSlimeManager* SpawningSlimeManagerOwner = Cast<ASpawningSlimeManager>(GetOwner()))
		{
			GameStateEventManager->OnActorSpawnedSendTransformOfSpawnPointDelegate.AddUniqueDynamic(
				SpawningSlimeManagerOwner, &ASpawningSlimeManager::SpawnSlimeAtTranform);
		}
	}
}
