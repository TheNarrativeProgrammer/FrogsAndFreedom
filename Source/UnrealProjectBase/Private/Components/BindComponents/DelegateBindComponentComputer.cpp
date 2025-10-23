// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentComputer.h"
#include "Environment/Generator/ComputerGeneratorActivator.h"

void UDelegateBindComponentComputer::BindDelegateFromBP()
{
}

void UDelegateBindComponentComputer::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AComputerGeneratorActivator* AComputerGeneratorActivatorOwner = Cast<AComputerGeneratorActivator>(GetOwner()))
		{
			GameStateEventManager->FOnForceFieldDeathDelegate.AddUniqueDynamic(AComputerGeneratorActivatorOwner, &AComputerGeneratorActivator::OnHealthDepletedOfGeneratorForcceFieldChangeComputerState);
			GameStateEventManager->OnEncounterStartedDelegate.AddUniqueDynamic(AComputerGeneratorActivatorOwner, &AComputerGeneratorActivator::OnEncounterStartBroadcastActorInitializationDelegates);
		}
	}
}
