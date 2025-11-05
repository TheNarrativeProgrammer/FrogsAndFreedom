// Copyright Peasant Pals 2025


#include "Environment/Generator/PowerCordManager.h"

#include "Environment/Generator/PowerCord.h"

// Sets default values
APowerCordManager::APowerCordManager()
{
	PrimaryActorTick.bCanEverTick = false;
	DelegateBindingComponentCordMgmr = CreateDefaultSubobject<
		UDelegateBindingComponentCordMgmr>("DelegateBindingComponentCordMgmr");
}

void APowerCordManager::TurnPowerCordsOn()
{
	for (APowerCord* PowerCord : _msPowerCordsInLevel)
	{
		if (!PowerCord) continue;
		PowerCord->TurnPowerCordsOn();
	}
}

void APowerCordManager::TurnPowerCordsOff()
{
	for (APowerCord* PowerCord : _msPowerCordsInLevel)
	{
		if (!PowerCord) continue;
		PowerCord->TurnPowerCordsOff();
	}
}


// Called when the game starts or when spawned
void APowerCordManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerCordManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

