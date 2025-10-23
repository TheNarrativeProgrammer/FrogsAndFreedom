// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/SpawningSlimeManager.h"

// Sets default values
ASpawningSlimeManager::ASpawningSlimeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DelegateBindComponentSlimeManger = CreateDefaultSubobject<UDelegateBindComponentSlimeManger>(TEXT("DelegateBindComponent"));
}

// Called when the game starts or when spawned
void ASpawningSlimeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawningSlimeManager::SpawnSlimeAtTranform_Implementation(FTransform SpawnTransform)
{
}

// Called every frame
void ASpawningSlimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

