// Copyright Peasant Pals 2025


#include "Weapons/PlayerWeapon/RocketLauncherComponent.h"
#include "Characters/Player/CharacterBasePlayer.h"
#include "TimerManager.h"

// Sets default values for this component's properties
URocketLauncherComponent::URocketLauncherComponent()
{
	// Set this component to be initialized when the game starts,
	PrimaryComponentTick.bCanEverTick = true;
	bCanFire = true;

	// ...
}

void URocketLauncherComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URocketLauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void URocketLauncherComponent::StartFiring()
{
	if (bCanFire) 
	{
		OnFireRocket(); //call BP fire logic
		//CanFire set to false in BP
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &URocketLauncherComponent::ResetCanFire, FireRate, false);
		//UE_LOG(LogTemp, Warning, TEXT("Fire"));
	}
}

void URocketLauncherComponent::StopFiring()
{
}

//void URocketLauncherComponent::OnFireRocket()
//{
//}


void URocketLauncherComponent::OnFireRocket_Implementation()
{
}

void URocketLauncherComponent::ResetCanFire()
{
	bCanFire = true;
	//UE_LOG(LogTemp, Warning, TEXT("Reset"));
}






