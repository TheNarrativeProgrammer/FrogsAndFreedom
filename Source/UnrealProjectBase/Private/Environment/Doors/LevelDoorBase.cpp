// Copyright Peasant Pals 2025


#include "Environment/Doors/LevelDoorBase.h"


// Sets default values
ALevelDoorBase::ALevelDoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	DelegateBindingComponentLevelDoor = CreateDefaultSubobject<UDelegateBindComponentLevelDoor>(TEXT("DelegateBindingComponentLevelDoor"));
	//SceneComponentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentRoot"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ALevelDoorBase::BeginPlay()
{
	Super::BeginPlay();
}


void ALevelDoorBase::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
}







