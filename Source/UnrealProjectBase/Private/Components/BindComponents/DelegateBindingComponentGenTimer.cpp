// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentGenTimer.h"
#include "Environment/Generator/ComputerGeneratorActivator.h"
#include "Environment/Generator/GeneratorTimerManager.h"


void UDelegateBindingComponentGenTimer::BindDelegateFromBP_Implementation()
{
	Super::BindDelegateFromBP_Implementation();
}

void UDelegateBindingComponentGenTimer::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AGeneratorTimerManager* GeneratorTimerManagerOwner =
			Cast<AGeneratorTimerManager>(GetOwner()))
		{
			GameStateEventManager->OnGameOverClearAllActiveTimersDelegate.AddUniqueDynamic(
				GeneratorTimerManagerOwner, &AGeneratorTimerManager::OnGameOverClearTimer);
		}
	}
}
