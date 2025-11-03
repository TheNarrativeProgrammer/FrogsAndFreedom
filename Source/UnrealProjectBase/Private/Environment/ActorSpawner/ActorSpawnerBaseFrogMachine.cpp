// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/ActorSpawnerBaseFrogMachine.h"

#include "DelayAction.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Frogs/Enemies/CharacterBaseFrogTank.h"

AActorSpawnerBaseFrogMachine::AActorSpawnerBaseFrogMachine()
{
	//default value
	BoxComponentDamageArea = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponentDamageArea"));
	TankSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TankSpawnLocation"));
	//set attachment
	BoxComponentDamageArea->SetupAttachment(StaticMeshComponent);
	TankSpawnLocation->SetupAttachment(StaticMeshComponent);
	//Delegates
	DelegateBindComponentSpawner = CreateDefaultSubobject<UDelegateBindComponentSpawner>(TEXT("DelegateBindComponentSpawner"));

	//Gameplay tag
	GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Environment.ActorSpawner.Machine")));
}

void AActorSpawnerBaseFrogMachine::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnHealthDepletedDelegate.AddUniqueDynamic(this, &AActorSpawnerBaseFrogMachine::Death);
	OnBeginplayBroadcastActorInitializationDelegates();
}

void AActorSpawnerBaseFrogMachine::OnTankDeathCheckIDMatches(int32 ActorID)
{
	if (ActorID == _mActorIDTankAtThisSpawner)
	{
		OnTankDeathRemoveForceField();
	}
}

FTransform AActorSpawnerBaseFrogMachine::GetTransformSpawnLocationForTank()
{
	return TankSpawnLocation->GetComponentTransform();
}

int32 AActorSpawnerBaseFrogMachine::GetActorIDTankAtThisSpawner_Implementation()
{
	return _mActorIDTankAtThisSpawner;
}

int32 AActorSpawnerBaseFrogMachine::GetActorIDOfSpawner_Implementation()
{
	return this->GetUniqueID();
}

void AActorSpawnerBaseFrogMachine::OnEncounterStartBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnEncounterStartBroadcastActorInitializationDelegates_Implementation();
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		if(DelegateBindComponentSpawner->GameStateEventManager)
		{
			DelegateBindComponentSpawner->GameStateEventManager->OnEncounterStartActorSpawnerBaseMachineInitialDelegate.Broadcast();
		}
	}
}

void AActorSpawnerBaseFrogMachine::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnBeginplayBroadcastActorInitializationDelegates_Implementation();
	// if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>(UGameplayStatics::GetGameState(GetWorld())))
	// {
	// 	if(DelegateBindComponentSpawner->GameStateEventManager)
	// 	{
	// 		DelegateBindComponentSpawner->GameStateEventManager->OnActorSpawnerBaseMachineInitalizedDelegate.Broadcast();
	// 	}
	// }
}


void AActorSpawnerBaseFrogMachine::SetTankLocation(FTransform InTankLocation, uint32 InActorID)
{
	if (InActorID == _mActorIDTankAtThisSpawner)
	{
		_mTankLocation = InTankLocation;
	}
}

void AActorSpawnerBaseFrogMachine::SetActorIDTankAtThisSpawner(uint32 ActorID)
{
	_mActorIDTankAtThisSpawner = ActorID;
	DelegateBindComponentSpawner->GameStateEventManager->OnActorSpawnerTankSpawnedDelegate.Broadcast(GetUniqueID());
}

bool AActorSpawnerBaseFrogMachine::GetHasTankFrog()
{
	return _mHasTankFrog;
}

void AActorSpawnerBaseFrogMachine::SetHasTankFrog(bool HasTankFrog)
{
	_mHasTankFrog = HasTankFrog;
}


bool AActorSpawnerBaseFrogMachine::GetCanTankSpawnHere()
{
	return _mCanTankSpawnHere;
}

void AActorSpawnerBaseFrogMachine::Death()
{
	SetCanSpawnActor(false);
	DelegateBindComponentSpawner->GameStateEventManager->OnActorSpawnerBaseMachineDeathDelegate.Broadcast();
	DelegateBindComponentSpawner->GameStateEventManager->OnActorSpawnerBaseMachineDeathSelfRefDelegate.Broadcast(this);
}

bool AActorSpawnerBaseFrogMachine::GetIsShielded_Implementation()
{
	return _mHasTankFrog;
}


