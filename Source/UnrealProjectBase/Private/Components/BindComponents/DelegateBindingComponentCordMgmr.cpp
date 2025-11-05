// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentCordMgmr.h"
#include "Environment/Generator/PowerCordManager.h"

void UDelegateBindingComponentCordMgmr::BindDelegateFromBP_Implementation()
{
	Super::BindDelegateFromBP_Implementation();
}

void UDelegateBindingComponentCordMgmr::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (APowerCordManager* PowerCordManagerOwner = Cast<APowerCordManager>(GetOwner()))
		{
			GameStateEventManager->OnAllForceFieldActivatedTurnPowerOnDelegate.AddUniqueDynamic(
				PowerCordManagerOwner, &APowerCordManager::TurnPowerCordsOn);
			
			GameStateEventManager->OnForceFieldDeactivatedTurnPowerOffDelegate.AddUniqueDynamic(
				PowerCordManagerOwner, &APowerCordManager::TurnPowerCordsOff);
		}
	}
}
