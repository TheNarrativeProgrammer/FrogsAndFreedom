// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentFrogTank.h"
#include "Characters/Frogs/Enemies/CharacterBaseFrogTank.h"

UDelegateBindingComponentFrogTank::UDelegateBindingComponentFrogTank()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDelegateBindingComponentFrogTank::BindDelegateFromBP()
{
}

void UDelegateBindingComponentFrogTank::BindDelegates()
{
	Super::BindDelegates();
}
