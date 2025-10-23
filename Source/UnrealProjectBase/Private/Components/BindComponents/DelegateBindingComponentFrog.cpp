// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentFrog.h"
#include "Characters/Frogs/CharacterBaseFrog.h"

void UDelegateBindingComponentFrog::BindDelegateFromBP()
{
}

void UDelegateBindingComponentFrog::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (ACharacterBaseFrog* ACharacterBaseFrogOwner = Cast<ACharacterBaseFrog>(GetOwner()))
		{
			//GameStateEventManager->FOnForceFieldDeathDelegate.AddUniqueDynamic(ACharacterBaseFrogOwner, &ACharacterBaseFrog::SomeFunctions);
		}
	}
}
