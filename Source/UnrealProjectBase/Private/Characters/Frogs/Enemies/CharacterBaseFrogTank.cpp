// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Frogs/Enemies/CharacterBaseFrogTank.h"

ACharacterBaseFrogTank::ACharacterBaseFrogTank()
{
	DelegateBindingComponentFrogTank = CreateDefaultSubobject<UDelegateBindingComponentFrogTank>(TEXT("DelegateBindingComponentFrogTank"));
}

void ACharacterBaseFrogTank::OnPoolActorActivated_Implementation()
{
	Super::OnPoolActorActivated_Implementation();
	
	IPoolable::OnPoolActorActivated_Implementation();
	if (IsValid((DelegateBindingComponentFrogTank)))
	{
		if (DelegateBindingComponentFrogTank->GameStateEventManager)
		{
			DelegateBindingComponentFrogTank->GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.Broadcast();
		}
		
	}
}


void ACharacterBaseFrogTank::OnLaunchTriggerFlinchAnim_Implementation()
{
}

void ACharacterBaseFrogTank::SendLocation()
{
	DelegateBindingComponentFrogTank->GameStateEventManager->OnCharacterBaseFrogTankSpawnedLocationDelegate.Broadcast(GetTransform(), GetUniqueID());
}

void ACharacterBaseFrogTank::Death()
{
	Super::Death();
	DelegateBindingComponentFrogTank->GameStateEventManager->OnCharacterBaseFrogTankDeathIDDelegate.Broadcast(GetUniqueID());
}
