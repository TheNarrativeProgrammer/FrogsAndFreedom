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
			GameStateEventManager->OnGeneratorInteractSendForceFieldIDDelegate.AddUniqueDynamic(
				ForceFieldOwner,
				&AForceFieldBase::OnGeneratorInteractRestoreHealthAndActivateIfIdMatches);
			
			GameStateEventManager->OnEncounterStartedDelegate.AddUniqueDynamic(
				ForceFieldOwner,
				&AForceFieldBase::OnEncounterStartBroadcastActorInitializationDelegates);
			
			GameStateEventManager->FOnEncounterEndedDelegate.AddUniqueDynamic(
				ForceFieldOwner, &AForceFieldBase::OnEncounterEndDestroyThis);
			
			GameStateEventManager->FOnForceFieldDeathDelegate.AddUniqueDynamic(
				ForceFieldOwner, &AForceFieldBase::DeactivateComponents);
		}
	}
}
