// Copyright Peasant Pals 2025


#include "Managers/FmodBusManager.h"

// Sets default values
AFmodBusManager::AFmodBusManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DelegateBindComponentFmodBusMmgr = CreateDefaultSubobject<UDelegateBindComponentFmodBusMmgr>(TEXT("DelegateBindComponentFmodBusMmgr"));
}

// Called when the game starts or when spawned
void AFmodBusManager::BeginPlay()
{
	Super::BeginPlay();
}

void AFmodBusManager::OnGameUnpauseUnpauseFmodBuses_Implementation()
{
}

void AFmodBusManager::OnGamePausePauseFmodBuses_Implementation()
{
}


