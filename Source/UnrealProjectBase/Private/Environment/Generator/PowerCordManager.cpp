// Copyright Peasant Pals 2025


#include "Environment/Generator/PowerCordManager.h"

#include "Environment/Generator/PowerCord.h"

// Sets default values
APowerCordManager::APowerCordManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DelegateBindingComponentCordMgmr = CreateDefaultSubobject<UDelegateBindingComponentCordMgmr>("DelegateBindingComponentCordMgmr");

}

void APowerCordManager::TurPowerCordsOn()
{
	for (APowerCord* PowerCord : _msPowerCordsInLevel)
	{
		PowerCord->TurnPowerCordsOn();
	}
}

void APowerCordManager::TurnPowerCordsOff()
{
	for (APowerCord* PowerCord : _msPowerCordsInLevel)
	{
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

