// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentForceField.h"
#include "Environment/Generator/ForceFieldBase.h"

void UDelegateBindComponentForceField::BindDelegateFromBP()
{
}

void UDelegateBindComponentForceField::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AForceFieldBase* ForceFieldOwner = Cast<AForceFieldBase>(GetOwner()))
		{
			//Old code that used Computer Generator Lever pull. Old Delegates had signature that didn't have any params. New Signature passes the ID of the Forcefield.
			//GameStateEventManager->OnComputerGeneratorLeverPullDelegate.AddUniqueDynamic(ForceFieldOwner, &AForceFieldBase::RestoreHealth);
			//GameStateEventManager->OnComputerGeneratorLeverPullDelegate.AddUniqueDynamic(ForceFieldOwner, &AForceFieldBase::OnHealthRestoredToDefaultActivateComponets);
			//Generator Interact
			GameStateEventManager->OnGeneratorInteractSendForceFieldIDDelegate.AddUniqueDynamic(ForceFieldOwner, &AForceFieldBase::OnGeneratorInteractRestoreHealthAndActivateIfIdMatches);

			//Encounter start
			GameStateEventManager->OnEncounterStartedDelegate.AddUniqueDynamic(ForceFieldOwner, &AForceFieldBase::OnEncounterStartBroadcastActorInitializationDelegates);
			//Encounber End - Destroy this
			GameStateEventManager->FOnEncounterEndedDelegate.AddUniqueDynamic(ForceFieldOwner, &AForceFieldBase::OnEncounterEndDestroyThis);

			//Forcefield Death
			GameStateEventManager->FOnForceFieldDeathDelegate.AddUniqueDynamic(ForceFieldOwner, &AForceFieldBase::DeactivateComponents);
			
			
		}
	}
}
