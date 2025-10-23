// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/PoolInitalizerBase.h"
#include "Subsystems/ObjectPoolingSubsystem.h"

// Sets default values
APoolInitalizerBase::APoolInitalizerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APoolInitalizerBase::BeginPlay()
{
	Super::BeginPlay();
	//Initialize pool with Actors of class
	InitalizePool(_mPoolOfClassDataFlusher);
	InitalizePool(_mPoolOfClassDataPinner);
	InitalizePool(_mPoolOfClassDataTank);
	
}

// Called every frame
void APoolInitalizerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APoolInitalizerBase::InitalizePool(const FPoolOfClassData& poolOfClassData)
{
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		ObjectPoolingSubsystem->PreWarmPool(poolOfClassData.ActorClassToSpawn, poolOfClassData.PopulationInPool, poolOfClassData.Transform);
	}
}

