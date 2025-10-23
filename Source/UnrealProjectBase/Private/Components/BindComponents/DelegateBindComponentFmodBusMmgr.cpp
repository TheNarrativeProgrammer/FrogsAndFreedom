// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentFmodBusMmgr.h"

#include "Managers/FmodBusManager.h"

void UDelegateBindComponentFmodBusMmgr::BindDelegates()
{
	Super::BindDelegates();

	if (GameStateEventManager)
	{
		if (AFmodBusManager* FmodBusManagerOwner = Cast<AFmodBusManager>(GetOwner()))
		{
			GameStateEventManager->OnGamePausedDelegate.AddUniqueDynamic(FmodBusManagerOwner, &AFmodBusManager::OnGamePausePauseFmodBuses);
			GameStateEventManager->OnGameUnPausedDelegate.AddUniqueDynamic(FmodBusManagerOwner, &AFmodBusManager::OnGameUnpauseUnpauseFmodBuses);
		}
	}
}

void UDelegateBindComponentFmodBusMmgr::BindDelegateFromBP_Implementation()
{
	Super::BindDelegateFromBP_Implementation();
}
