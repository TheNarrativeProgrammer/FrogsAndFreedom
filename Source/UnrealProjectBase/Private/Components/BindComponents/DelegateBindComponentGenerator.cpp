// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentGenerator.h"
#include "Environment/Generator/GeneratorBase.h"

void UDelegateBindComponentGenerator::BindDelegateFromBP()
{
}

void UDelegateBindComponentGenerator::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AGeneratorBase* GeneratorBaseOwner = Cast<AGeneratorBase>(GetOwner()))
		{
			GameStateEventManager->OnForceFieldBaseDeathIDDelegate.AddUniqueDynamic(
				GeneratorBaseOwner, &AGeneratorBase::CheckForceFieldIDMatchesThisOwner);
			
			GameStateEventManager->OnEncounterStartedDelegate.AddUniqueDynamic(
				GeneratorBaseOwner,
				&AGeneratorBase::OnEncounterStartBroadcastActorInitializationDelegates);
		}
	}
}
