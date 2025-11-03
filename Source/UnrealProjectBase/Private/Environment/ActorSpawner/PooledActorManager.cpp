// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/PooledActorManager.h"
#include "DelayAction.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Characters/Frogs/Enemies/CharacterBaseFrogTank.h"
#include "Environment/ActorSpawner/ActorSpawnerBaseFrogMachine.h"
#include "Environment/ActorSpawner/ActorSpawnerBaseVent.h"
#include "Subsystems/ObjectPoolingSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APooledActorManager::APooledActorManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APooledActorManager::BeginPlay()
{
	Super::BeginPlay();

	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>( UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
	}
	if (GameStateEventManager)
	{
		//Spawning Delegates
		//GameStateEventManager->OnActorSpawnerBaseMachineInitalizedDelegate.AddUniqueDynamic(this, &APooledActorManager::IncreaseMachineCount);
		//GameStateEventManager->OnActorSpawnerBaseInitalizedSelfRefDelegate.AddUniqueDynamic(this, &APooledActorManager::OnInitializedAddActorSpawnerBaseToArray);
		//GameStateEventManager->OnActorSpawnerBaseMachineDeathSelfRefDelegate.AddUniqueDynamic(this, &APooledActorManager::RemoveDeadMachineFromArray);
		//GameStateEventManager->OnActorSpawnerBaseMachineDeathDelegate.AddUniqueDynamic(this, &APooledActorManager::DecreaseMachineCount);

		
		//Enemy Delegates
		GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.AddUniqueDynamic(this, &APooledActorManager::IncreaseCurrentCharactersInLevel);
		GameStateEventManager->OnCharacterBaseFrogDeathDelegate.AddUniqueDynamic(this, &APooledActorManager::DecreaseCurrentCharactersInLevel);
		GameStateEventManager->OnCharacterBaseFrogDeathDelegate.AddUniqueDynamic(this, &APooledActorManager::CheckCharacterQuantityAndTriggerSpawn);
		//self ref to call initial spawn functions after others have initialized
		//GameStateEventManager->OnPooledActorManagerInitializedDelegate.AddUniqueDynamic(this, &APooledActorManager::CallLevelStartFunctions);
	}
	SetEnemyRatios();
	CheckForZeroDivisionAdjustSpawnBatchSize();
	
	//If Designer forgot to put number for batch size, this afgust batch size to that population at start of game.
	CheckForZeroBatchSizeWhenStartPopulationAboveZero(_mPooledClassSpawningDataFlusher);
	CheckForZeroBatchSizeWhenStartPopulationAboveZero(_mPooledClassSpawningDataPinner);
	CheckForZeroBatchSizeWhenStartPopulationAboveZero(_mPooledClassSpawningDataTank);

	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		//GameStateEventManager->OnPooledActorManagerInitializedDelegate.Broadcast();
	});
	//CallLevelStartFunctions();
}



void APooledActorManager::CallLevelStartFunctions()
{
	//populate array with Machines in level.
	// if (_maActorSpawnerBaseFrogMachines.IsEmpty() == false)
	// {
	// 	RemoveListenersToHealthOfActorSpawners();
	// }
	
	//PopulateArrayWithActorsOfClass(AActorSpawnerBaseFrogMachine::StaticClass(), _maActorSpawnerBaseFrogMachines);
	//PopulateArrayWithActorsOfClass(AActorSpawnerBaseVent::StaticClass(), _maActorSpawnerBaseVents);
	
	//populate array with struct data types. The defaults of these should be set in the details of the INSTANCE
	// _maPooledClassSpawningData =
	// 	{
	// 	_mPooledClassSpawningDataFlusher,
	// 	_mPooledClassSpawningDataPinner,
	// 	_mPooledClassSpawningDataTank
	// 	};
	
	//Register listeners to HealthComponent of Machines. Iniitalize the variables for delegate broadcast that occurs when
	//machine becomes inactive. Send initial broadcast of quanity of machines in level and quantity remaining active. 
	//RegisterListenersToHealthOfActorSpawners();
	//SendInitialBroadcast();
	
	//Initialize pool with Actors of class
	// InitalizePool(_mPoolOfClassDataFlusher);
	// InitalizePool(_mPoolOfClassDataPinner);
	// InitalizePool(_mPoolOfClassDataTank);

	// if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	// {
	// 	if (ObjectPoolingSubsystem->GetTotalFrogsInGamePlaySingleClass(_mPooledClassSpawningDataFlusher.ActorClassToSpawn) > 0)
	// 	{
	// 		RemoveListenersToHealthOfCharacters(_mPooledClassSpawningDataFlusher.ActorClassToSpawn);
	// 	}
	// 	if (ObjectPoolingSubsystem->GetTotalFrogsInGamePlaySingleClass(_mPooledClassSpawningDataPinner.ActorClassToSpawn) > 0)
	// 	{
	// 		RemoveListenersToHealthOfCharacters(_mPooledClassSpawningDataPinner.ActorClassToSpawn);
	// 	}
	// 	if (ObjectPoolingSubsystem->GetTotalFrogsInGamePlaySingleClass(_mPooledClassSpawningDataTank.ActorClassToSpawn) > 0)
	// 	{
	// 		RemoveListenersToHealthOfCharacters(_mPooledClassSpawningDataTank.ActorClassToSpawn);
	// 	}
	// }

	//Register Listeners to health component delegates of Characters
	//RegisterListenersToHealthOfCharacters(_mPooledClassSpawningDataFlusher.ActorClassToSpawn);
	//RegisterListenersToHealthOfCharacters(_mPooledClassSpawningDataPinner.ActorClassToSpawn);
	//RegisterListenersToHealthOfCharacters(_mPooledClassSpawningDataTank.ActorClassToSpawn);

	//Spawn initial number of Actors from pool of a given class
	OnGameStartPopulateWorld(_mPooledClassSpawningDataFlusher);
	OnGameStartPopulateWorld(_mPooledClassSpawningDataPinner);
	OnGameStartPopulateWorld(_mPooledClassSpawningDataTank);
}

void APooledActorManager::SetEnemyRatios()
{
	switch (_mEnemyRatio)
	{
	case eEnemyRatio::Flusher_100___Pinner_0:
		{
			_mPooledClassSpawningDataFlusher.EnemyRatio = 1;
			_mPooledClassSpawningDataPinner.EnemyRatio = 0;
			break;
		}
	case eEnemyRatio::Flusher_80____Pinner_20:
		{
			_mPooledClassSpawningDataFlusher.EnemyRatio = 0.8;
			_mPooledClassSpawningDataPinner.EnemyRatio = 0.2;
			break;
		}
	case eEnemyRatio::Flusher_60____Pinner_40:
		{
			_mPooledClassSpawningDataFlusher.EnemyRatio = 0.6;
			_mPooledClassSpawningDataPinner.EnemyRatio = 0.4;
			break;
		}
	case eEnemyRatio::Flusher_40____Pinner_60:
		{
			_mPooledClassSpawningDataFlusher.EnemyRatio = 0.4;
			_mPooledClassSpawningDataPinner.EnemyRatio = 0.6;
			break;
		}
	case eEnemyRatio::Flusher_20____Pinner_80:
		{
			_mPooledClassSpawningDataFlusher.EnemyRatio = 0.2;
			_mPooledClassSpawningDataPinner.EnemyRatio = 0.8;
			break;
		}
	case eEnemyRatio::Flusher_0_____Pinner_100:
		{
			_mPooledClassSpawningDataFlusher.EnemyRatio = 0;
			_mPooledClassSpawningDataPinner.EnemyRatio = 1;
			break;
		}
	}
}

void APooledActorManager::CheckForZeroDivisionAdjustSpawnBatchSize()
{
	if (_mPooledClassSpawningDataFlusher.EnemyRatio > 0 && _mPooledClassSpawningDataFlusher.SpawnBatchSizeAtSingleLocation == 0)
	{
		_mPooledClassSpawningDataFlusher.SpawnBatchSizeAtSingleLocation = 1;
	}

	if (_mPooledClassSpawningDataPinner.EnemyRatio > 0 && _mPooledClassSpawningDataPinner.SpawnBatchSizeAtSingleLocation == 0)
	{
		_mPooledClassSpawningDataPinner.SpawnBatchSizeAtSingleLocation = 1;
	}
	
}

void APooledActorManager::CheckForZeroBatchSizeWhenStartPopulationAboveZero(FPooledClassSpawningData& PooledClassSpawningData)
{
	if (PooledClassSpawningData.PopulationGameStart > 0 && PooledClassSpawningData.SpawnBatchSizeAtSingleLocation == 0)
	{
		PooledClassSpawningData.SpawnBatchSizeAtSingleLocation = PooledClassSpawningData.PopulationGameStart;
	}
}


//new function to replace health component binding updating count
void APooledActorManager::IncreaseCurrentCharactersInLevel()
{
	_mCurrentCharatersInLevel++;
}

void APooledActorManager::DecreaseCurrentCharactersInLevel()
{
	_mCurrentCharatersInLevel--;
}


void APooledActorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




void APooledActorManager::PopulateArrayWithActorsOfClass(TSubclassOf<AActorSpawnerBase> classToSearchFor,
	TArray<FActorSpawnerBaseData>& inArrayToPopulate)
{
	
	// TArray<AActor*> FoundActors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToSearchFor, FoundActors);
	//
	// if (FoundActors.Num() <= 0)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("There are zero ActorSpawnerBases. Add some to level "));
	// }
	//
	// for (AActor* Actor : FoundActors)
	// {
	// 	AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(Actor);
	// 	if (ActorSpawnerBase)
	// 	{
	// 		FActorSpawnerBaseData ActorSpawnerBaseData;
	// 		ActorSpawnerBaseData.Actor = ActorSpawnerBase;
	// 		if (USceneComponent* SceneComponent = Cast<USceneComponent>(ActorSpawnerBase->GetComponentByClass(USceneComponent::StaticClass())))
	// 		{
	// 			ActorSpawnerBaseData.Transform = SceneComponent->GetComponentTransform();
	// 		}
	// 		
	// 		ActorSpawnerBaseData.CharacterSpawnType = ActorSpawnerBase->GetSpawnType();
	// 		ActorSpawnerBaseData.DistanceThisToPlayer = ActorSpawnerBase->GetDistanceToPlayer();
	// 		ActorSpawnerBaseData.DotProductToPlayer = ActorSpawnerBase->GetDotProductToPlayer();
	//
	// 		inArrayToPopulate.Add(ActorSpawnerBaseData);
	// 	}
	// }
}



// void APooledActorManager::RegisterListenersToHealthOfActorSpawners()
// {
// 	for (FActorSpawnerBaseData& ActorSpawnerBaseData : _maActorSpawnerBaseFrogMachines)
// 	{
// 		if (AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(ActorSpawnerBaseData.Actor))
// 		{
// 			ActorSpawnerBase->GetHealthComponent()->OnHealthDepletedSelfRefDelegate.AddDynamic(this, &APooledActorManager::RemoveInactiveMachineFromArray);
// 			
// 		}
// 	}
// }

// void APooledActorManager::RemoveListenersToHealthOfActorSpawners()
// {
// 	for (FActorSpawnerBaseData& ActorSpawnerBaseData : _maActorSpawnerBaseFrogMachines)
// 	{
// 		if (AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(ActorSpawnerBaseData.Actor))
// 		{
// 			ActorSpawnerBase->GetHealthComponent()->OnHealthDepletedSelfRefDelegate.RemoveAll(this);
// 			
// 		}
// 	}
// }

// void APooledActorManager::RegisterListenersToHealthOfCharacters(TSubclassOf<AActor> ActorClass)
// {
// 	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
// 	{
// 		//Use class key to get the struct containing all the Actors of that class in the pool
// 		if (FActorArrayWrapper* ActorArrayWrapper = ObjectPoolingSubsystem->_mdActorPools.Find(ActorClass))
// 		{
// 			//Iterate through each actor within strut. Cast to ACharacterBase to access healthComponent. Subscrube to Health deligates
// 			for (AActor* ActorInPool : ActorArrayWrapper->ActorsInPoolOrLevel)
// 			{
// 				if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(ActorInPool))
// 				{
// 					if (UHealthComponent* HealthComponent =  CharacterBase->HealthComponent)
// 					{
// 						HealthComponent->OnHealthDepleatedDecrementCountDelegate.AddUniqueDynamic(this, &APooledActorManager::UpdateCurrentCharatersInLevel);
// 						HealthComponent->OnHealthDepletedDelegate.AddUniqueDynamic(this, &APooledActorManager::CheckCharacterQuantityAndTriggerSpawn);
// 						UE_LOG(LogTemp, Warning, TEXT("Binding Health depleated for %s"), *CharacterBase->GetName());
// 						//FString message = *CharacterBase->GetName();
// 						//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, "Binding delegate");
// 						//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, message);
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("There is no Actor Class from in _mdActorPools"));
// 			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, TEXT("There is no Actor Class from in _mdActorPools"));
// 		}
// 	}
// }

// void APooledActorManager::RemoveListenersToHealthOfCharacters(TSubclassOf<AActor> ActorClass)
// {
// 	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
// 	{
// 		//Use class key to get the struct containing all the Actors of that class in the pool
// 		if (FActorArrayWrapper* ActorArrayWrapper = ObjectPoolingSubsystem->_mdActorPools.Find(ActorClass))
// 		{
// 			//Iterate through each actor within strut. Cast to ACharacterBase to access healthComponent. Subscrube to Health deligates
// 			for (AActor* ActorInPool : ActorArrayWrapper->ActorsInPoolOrLevel)
// 			{
// 				if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(ActorInPool))
// 				{
// 					if (UHealthComponent* HealthComponent =  CharacterBase->HealthComponent)
// 					{
// 						HealthComponent->OnHealthDepleatedDecrementCountDelegate.RemoveAll(this);
// 						HealthComponent->OnHealthDepletedDelegate.RemoveAll(this);
// 						UE_LOG(LogTemp, Warning, TEXT("Binding Health depleated for %s"), *CharacterBase->GetName());
// 						//FString message = *CharacterBase->GetName();
// 						//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, "Binding delegate");
// 						//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, message);
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("There is no Actor Class from in _mdActorPools"));
// 			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, TEXT("There is no Actor Class from in _mdActorPools"));
// 		}
// 	}
// }


// void APooledActorManager::InitializeDelegateVariables()
// {
// 	_mTotalMachineInLevel = _maActorSpawnerBaseFrogMachines.Num();
// 	_mRemainingMachineInLevel = _mTotalMachineInLevel;
// }

// void APooledActorManager::IncreaseMachineCount()
// {
// 	_mTotalMachineInLevel ++;
// 	_mRemainingMachineInLevel = _mTotalMachineInLevel;
// }

// void APooledActorManager::DecreaseMachineCount()
// {
// 	_mRemainingMachineInLevel--;
// }

// void APooledActorManager::SendInitialBroadcast()
// {
// 	InitializeDelegateVariables();
// 	MachineCountDelegate.Broadcast(_mTotalMachineInLevel, _mRemainingMachineInLevel);
// }


// void APooledActorManager::InitalizePool(const FPoolOfClassData& poolOfClassData)
// {
// 	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
// 	{
// 		ObjectPoolingSubsystem->PreWarmPool(poolOfClassData.ActorClassToSpawn, poolOfClassData.PopulationInPool, poolOfClassData.Transform);
// 	}
// }



//todo REMOVE THIS CODE. replaced by RemoveDeadMachineFromArry
void APooledActorManager::RemoveInactiveMachineFromArray(AActor* ActorSpawnerBaseToRemove)
{
	int32 IndexToRemove = INDEX_NONE;
	
	for (int32 i = 0; i < _maActorSpawnerBaseFrogMachines.Num(); i++)
	{
		if (_maActorSpawnerBaseFrogMachines[i].Actor->GetActorNameOrLabel() == ActorSpawnerBaseToRemove->GetActorNameOrLabel())
		{
			IndexToRemove = i;
			break;
		}
	}
	if (IndexToRemove != INDEX_NONE)
	{
		_maActorSpawnerBaseFrogMachines.RemoveAt(IndexToRemove);
	}
	//UpdateMachineCount();
	//BroadCastMachineCount();
	
}
//
// void APooledActorManager::UpdateMachineCount()
// {
// 	_mRemainingMachineInLevel = _maActorSpawnerBaseFrogMachines.Num();
// }

// void APooledActorManager::BroadCastMachineCount()
// {
// 	MachineCountDelegate.Broadcast(_mTotalMachineInLevel, _mRemainingMachineInLevel);
// }

void APooledActorManager::SpawnActor(TSubclassOf<AActor> ActorClassToSpawn,
                                     FTransform ActorSpawnerBaseTransform)
{
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		ObjectPoolingSubsystem->SpawnActorFromPool(ActorClassToSpawn, ActorSpawnerBaseTransform);
	}
	//UpdateCurrentCharatersInLevel(1);
}



FTransform APooledActorManager::GetTransformOfIdealActorSpawner(FPooledClassSpawningData& PooledClassSpawningData, int32 indexDecrementOfSpawner)
{
	FTransform SpawnTransform;
	TArray<FActorSpawnerBaseData*> ActorSpawnerBaseOfClassType;
	for (FActorSpawnerBaseData& ActorSpawnerBaseData : _maActorSpawnerBaseFrogMachines)
	{
		//Update Struct data with data from ActorSpawnerBase
		//ActorSpawnerBaseData.DotProductToPlayer = Cast<AActorSpawnerBase>(ActorSpawnerBaseData.Actor)->GetDotProductToPlayer();
		ActorSpawnerBaseData.CanSpawnActor = Cast<AActorSpawnerBase>(ActorSpawnerBaseData.Actor)->GetCanSpawnActor();

		if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(ActorSpawnerBaseData.Actor))
		{
			ActorSpawnerBaseData.IsTankHere = ActorSpawnerBaseFrogMachine->GetHasTankFrog();
			ActorSpawnerBaseData.CanTankSpawnHere = ActorSpawnerBaseFrogMachine->GetCanTankSpawnHere();
		}
		
		//Check if character type being spawned is tank. Check if Machine has stats required for spawning tank.
		//If character Type is Tank, then all ActorSpawners are avialable. Filiter out any that already have Tank
		if (PooledClassSpawningData.CharacterType == eActorTypeToSpawn::Tank)
		{
			if (ActorSpawnerBaseData.IsTankHere == false
				&& ActorSpawnerBaseData.DotProductToPlayer >= PooledClassSpawningData.MimumumDotProductPlayerToSpawner
				&& ActorSpawnerBaseData.CanSpawnActor == true
				&& ActorSpawnerBaseData.CanTankSpawnHere == true)
			{
				ActorSpawnerBaseOfClassType.Add(&ActorSpawnerBaseData);
			}
		}
		
		
		//Test if ActorSpawnerBase is of the right characterType and CanSpawnActor (not damaged)
		else if (ActorSpawnerBaseData.CharacterSpawnType == PooledClassSpawningData.CharacterType
				&& ActorSpawnerBaseData.DotProductToPlayer >= PooledClassSpawningData.MimumumDotProductPlayerToSpawner
				&& ActorSpawnerBaseData.CanSpawnActor == true)
		{
				ActorSpawnerBaseOfClassType.Add(&ActorSpawnerBaseData);
		}
	}

	//If the game start population of tanks is greater than the number of spawners with CanSpawnTank set to true,
	//reduce the population to match quantity of spawners.
	if (PooledClassSpawningData.CharacterType == eActorTypeToSpawn::Tank
			&& PooledClassSpawningData.PopulationGameStart > ActorSpawnerBaseOfClassType.Num())
	{
		//UE_LOG(LogTemp, Warning, TEXT("DESIGNER NOTE: game population is greater than the number of spawners. Add more Spawners and set CanSpawnTank to true or decrease start population of tanks."));
	}

	//if list is empty, then exist.
	if (ActorSpawnerBaseOfClassType.IsEmpty() == true)
	{
		FString ActorClassName = PooledClassSpawningData.ActorClassToSpawn->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("Array of active spawn manchine in PoolActorManager is empty for type %s"), *ActorClassName);

		//Transform of 0's used to identify that no spawn locations exist and an actor can't be spawned
		if (_maActorSpawnerBaseVents.IsEmpty() == true)
		{
			SpawnTransform = FTransform::Identity;
			return SpawnTransform;
		}
		if (_maActorSpawnerBaseVents.IsValidIndex(indexDecrementOfSpawner) == true)
		{
			return _maActorSpawnerBaseVents[indexDecrementOfSpawner].Transform;
		}
			return _maActorSpawnerBaseVents[0].Transform;
	}

	
	ActorSpawnerBaseOfClassType.Sort();
	
	switch (PooledClassSpawningData.CharacterType)
	{
	case eActorTypeToSpawn::Flusher:
		{
			//get middle distance
			if (ActorSpawnerBaseOfClassType.IsEmpty() == false)
			{
				int32 middleIndex = (int)((ActorSpawnerBaseOfClassType.Num()/2));
				int32 middleIndexIncrument = middleIndex - indexDecrementOfSpawner;
				if (middleIndexIncrument >= 0)
				{
					AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(ActorSpawnerBaseOfClassType[middleIndexIncrument]->Actor);
					SpawnTransform = ActorSpawnerBase->GetTransformSpawneLocation();
					break;
				}
				else if (middleIndex >= 0)
				{
					AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(ActorSpawnerBaseOfClassType[middleIndex]->Actor);
					SpawnTransform = ActorSpawnerBase->GetTransformSpawneLocation();
					break;
				}
				//if both these fail, then do to default and choose a random available spawn point.
			}
			
		}
	case eActorTypeToSpawn::Pinner:
		{
			if (ActorSpawnerBaseOfClassType.IsEmpty() == false)
			{
				//get furtherest distance
				int32 furtherestIndex = ActorSpawnerBaseOfClassType.Num() - 1;
				int32 furtherestIndexIncrumment = furtherestIndex - indexDecrementOfSpawner;
				if (furtherestIndexIncrumment >= 0)
				{
					AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(ActorSpawnerBaseOfClassType[furtherestIndexIncrumment]->Actor);
					SpawnTransform = ActorSpawnerBase->GetTransformSpawneLocation();
					break;
				}
				else if (furtherestIndex >= 0)
				{
					AActorSpawnerBase* ActorSpawnerBase = Cast<AActorSpawnerBase>(ActorSpawnerBaseOfClassType.Last()->Actor);
					SpawnTransform = ActorSpawnerBase->GetTransformSpawneLocation();
					break;
				}
			}
		}
	case eActorTypeToSpawn::Tank:
		{
			if (ActorSpawnerBaseOfClassType.IsEmpty() == false)
			{
				//Use index 0 here. Only 1 tank can spawn at a SpawnMachine. The Machines with tanks are not added to the array
				//only possible spawn locations and thus the index automatically iterates.
				AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(ActorSpawnerBaseOfClassType[0]->Actor);
				SpawnTransform = ActorSpawnerBaseFrogMachine->GetTransformSpawneLocation();
				ActorSpawnerBaseFrogMachine->SetHasTankFrog(true);
				//save ptr of ActorSpawnerBase so it can be passed reference of the Tank spawned at it's location
				ActorSpawnerBaseWithTank = ActorSpawnerBaseFrogMachine;
				break;
			}
		}
	case eActorTypeToSpawn::All:
		{
			//UE_LOG(LogTemp, Warning, TEXT("A ActorSpawnBaseFrogMachine is set to ALL. Can only be Pinner or Flusher"));
			break;
		}
	default:
		{
			//Set Transform to zero if nothing matches switch.
			SpawnTransform = FTransform::Identity;
			//UE_LOG(LogTemp, Warning, TEXT("Default in switch statement. PoolActorManager for class %s"), *PooledClassSpawningData.ActorClassToSpawn->GetName());
			break;
		}
	}
	return SpawnTransform;
}

FTransform APooledActorManager::GetTransformActorSpawner(FPooledClassSpawningData& PooledClassSpawningData,
	int32 indexDecrement)
{
	//_maActorSpawnerBaseFrogMachinesFlusher
	FTransform SpawnTransform = FTransform::Identity;
	

	
	switch (PooledClassSpawningData.CharacterType)
	{
		case eActorTypeToSpawn::Flusher:
			{
				//if index is outside of the array length, loop back to start.
				if (indexDecrement >= _maActorSpawnerBaseFrogMachinesFlusher.Num() && _maActorSpawnerBaseFrogMachinesFlusher.IsEmpty() == false)
				{
					indexDecrement = indexDecrement % _maActorSpawnerBaseFrogMachinesFlusher.Num();
				}
				
				for (int i = indexDecrement; i < _maActorSpawnerBaseFrogMachinesFlusher.Num(); i++)
				{
					if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(_maActorSpawnerBaseFrogMachinesFlusher[i]))
					{
						if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == true)
						{
							SpawnTransform = ActorSpawnerBaseFrogMachine->GetTransformSpawneLocation();
							break;
						}
						
					}
				}
				break;
			}
	case eActorTypeToSpawn::Pinner:
			{
				//if index is outside of the array length, loop back to start.
				if (indexDecrement >= _maActorSpawnerBaseFrogMachinesPinner.Num() && _maActorSpawnerBaseFrogMachinesPinner.IsEmpty() == false)
				{
					indexDecrement = indexDecrement % _maActorSpawnerBaseFrogMachinesPinner.Num();
				}
				for (int i = indexDecrement; i < _maActorSpawnerBaseFrogMachinesPinner.Num(); i++)
				{
					if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(_maActorSpawnerBaseFrogMachinesPinner[i]))
					{
						if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == true)
						{
							SpawnTransform = ActorSpawnerBaseFrogMachine->GetTransformSpawneLocation();
							break;
						}
						
					}
				}
				break;
			}
	case eActorTypeToSpawn::Tank:
			{
				//if index is outside of the array length, loop back to start.
				if (indexDecrement >= _maActorSpawnerBaseFrogMachinesTank.Num() && _maActorSpawnerBaseFrogMachinesTank.IsEmpty() == false)
				{
					indexDecrement = indexDecrement % _maActorSpawnerBaseFrogMachinesTank.Num();
				}
				for (int i = indexDecrement; i < _maActorSpawnerBaseFrogMachinesTank.Num(); i++)
				{
					if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(_maActorSpawnerBaseFrogMachinesTank[i]))
					{
						if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == true
							&& ActorSpawnerBaseFrogMachine->GetHasTankFrog() == false
							&& ActorSpawnerBaseFrogMachine->GetCanTankSpawnHere() == true)
						{
							SpawnTransform = ActorSpawnerBaseFrogMachine->GetTransformSpawneLocation();
							ActorSpawnerBaseFrogMachine->SetHasTankFrog(true);
							//save ptr of ActorSpawnerBase so it can be passed reference of the Tank spawned at it's location
							ActorSpawnerBaseWithTank = ActorSpawnerBaseFrogMachine;
							break;
						}
					}
				}
				break;
			}
		default:
			{
				break;
			}
	}
	
	//If no machines available and there are vents, then iterate through the vents array
	if (SpawnTransform.GetLocation() == FVector::Zero())
	{
		//if no Machines available and no vents in level, then retrn Tranform::identitty. This is used as flag to skip spawning more frogs of this type.
		if (_maActorSpawnerBaseVent.IsEmpty())
		{
			return SpawnTransform;
		}

		//if index is outside of the array length, loop back to start.
		if (indexDecrement >= _maActorSpawnerBaseVent.Num() && _maActorSpawnerBaseVent.IsEmpty() == false)
		{
			indexDecrement = indexDecrement % _maActorSpawnerBaseVent.Num();
		}
		
		for (int i = indexDecrement; i <= _maActorSpawnerBaseVent.Num(); i++)
		{
			if (AActorSpawnerBaseVent* ActorSpawnerBase = Cast<AActorSpawnerBaseVent>(_maActorSpawnerBaseVent[i]))
			{
				SpawnTransform = ActorSpawnerBase->GetTransformSpawneLocation();
				break;
			}
		}
	}
	
	
	return SpawnTransform;
}

void APooledActorManager::OnGameStartPopulateWorld(FPooledClassSpawningData& PooledClassSpawningData)
{
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		int32 indexDecrementOfSpawner = 0;

		int32 BatchSize = GetLowestStartPopulationVsBatchSize(PooledClassSpawningData);

		SpawnCountTotalAllActorsOfClass = 0;
		while (SpawnCountTotalAllActorsOfClass < PooledClassSpawningData.PopulationGameStart)
		{
			//TODO remove classPopluationInGame after testing. This is to check the current population but it's not required 
			int32 classPopluationInGame = ObjectPoolingSubsystem->GetTotalFrogsInGamePlaySingleClass(PooledClassSpawningData.ActorClassToSpawn);
			//Find ideal spawn location
			//FTransform Transform = GetTransformOfIdealActorSpawner(PooledClassSpawningData, indexDecrementOfSpawner);

			FTransform Transform = GetTransformActorSpawner(PooledClassSpawningData, indexDecrementOfSpawner);

			//return early if there are no available Spawning Machines. GetTransformOfIdealActorSpawner returns 0 if none available
			if (Transform.GetLocation() == FVector::Zero())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Exit OnGameStartPopulateWorld early. No actorSpawners for class %s"), *PooledClassSpawningData.ActorClassToSpawn->GetName());
				return;
			}


			
			indexDecrementOfSpawner++;
			SpawnCountOfBatch = 0;
			
			//Set member variable struct to data for current batch. This is persistent data used in SpawnActorFromBatch()
			BatchSpawningDataCurrentBatch.PooledClassSpawningData = PooledClassSpawningData;
			BatchSpawningDataCurrentBatch.SpawnTransform = Transform;

			//Check for infinite loop
			if (BatchSize <= 0)
			{
				return;
			}

			if (ActorSpawnerBaseWithTank == nullptr && PooledClassSpawningData.CharacterType == eActorTypeToSpawn::Tank
				&& PooledClassSpawningData.PopulationGameStart > 0)
			{
				//UE_LOG(LogTemp, Warning, TEXT("No ActorSpawnerMachines have CanSpawnTank set to true"));
				return;
			}
			

			//Spawn an individual actor of the current batch
			while (SpawnCountOfBatch < BatchSize)
			{
				//SpawnActorFromBatch(PooledClassSpawningData, indexDecrementOfSpawner, Transform);
				SpawnActorFromBatch();
			}
		}
		
	}
	
}

void APooledActorManager::SpawnBatch(FPooledClassSpawningData& PooledClassSpawningData, FTransform SpawnTransform)
{
	if (_mCurrentCharatersInLevel <=_maxCharatersInLevel)
	{

		//Set member variable struct to data for current batch. This is persistent data used in SpawnActorFromBatch()
		BatchSpawningDataCurrentBatch.PooledClassSpawningData = PooledClassSpawningData;
		BatchSpawningDataCurrentBatch.SpawnTransform = SpawnTransform;

		//TODO REMOVE THIS WHILE LOOP AND REPLACE WITH TIMER
		SpawnCountOfBatch = 0;

		//todo remove code below. Tanks should only spawn when level loads.
		
		//The max number of Tanks that can spawn at a single location can't exceed 1.
		// if (BatchSpawningDataCurrentBatch.PooledClassSpawningData.CharacterType == eActorTypeToSpawn::Tank
		// 	&& BatchSpawningDataCurrentBatch.PooledClassSpawningData.SpawnBatchSizeAtSingleLocation > 1)
		// {
		// 	BatchSpawningDataCurrentBatch.PooledClassSpawningData.SpawnBatchSizeAtSingleLocation = 1;
		// }
		
		while (SpawnCountOfBatch < BatchSpawningDataCurrentBatch.PooledClassSpawningData.SpawnBatchSizeAtSingleLocation)
		{
			SpawnActorFromBatch();
		}
		
		// GetWorld()->GetTimerManager().SetTimer(
		// 	SpawnTimerHandle,
		// 	this,
		// 	&APooledActorManager::SpawnActorFromBatch,
		// 	0.5f,
		// 	true);

		//UE_LOG(LogTemp, Display, TEXT("Timer set %s"), GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) ? TEXT("ACTIVE") : TEXT("NOT ACTIVE"));

		if (IsValid(this))
		{
			//UE_LOG(LogTemp, Display, TEXT("Pool actor manager is valid"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Pool actor manager is NOT valid"));
		}
	}
}

void APooledActorManager::SpawnActorFromBatch()
{
	//UE_LOG(LogTemp, Display, TEXT("Spawn Actor from batch fired"));
	//clear timer if batch max is reached
	// if (SpawnCountOfBatch >= BatchSpawningDataCurrentBatch.PooledClassSpawningData.SpawnBatchSizeAtSingleLocation)
	// {
	// 	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	// 	
	// 	return;
	// }
	//Find ideal spawn location
	//FTransform Transform = GetTransformOfIdealActorSpawner(PooledClassSpawningData, indexDecrementOfSpawner);

	//Remove actor from pool and spawn at location.
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		if (AActor* pooledActor = ObjectPoolingSubsystem->SpawnActorFromPool(
			BatchSpawningDataCurrentBatch.PooledClassSpawningData.ActorClassToSpawn,
			BatchSpawningDataCurrentBatch.SpawnTransform))
		{
			//if spawned Actor is a tank, pass a ptr of the Tank to the ActorSpawner. This is used to determine if force field applied
			if (ACharacterBaseFrogTank* CharacterBaseFrogTank = Cast<ACharacterBaseFrogTank>(pooledActor))
			{
				//todo try to docouple OnSetTankSetForceFieldLocation(); this is temp solution
				ActorSpawnerBaseWithTank->SetActorIDTankAtThisSpawner(CharacterBaseFrogTank->GetUniqueID());
				ActorSpawnerBaseWithTank->OnSetTankSetForceFieldLocation();
				//ActorSpawnerBaseWithTank->SetFrogTankAtThisMachine(CharacterBaseFrogTank);
				//ActorSpawnerBaseWithTank->RegisterListenerToTankDeathDelegate();
				//reset persistent reference to the last ActorSpawnerBaseWithTank. There are null checks to see if this is valid
				//and it's updated only when a new tank spawns at a ActorSpawnerBaseMachine.
				//If there are not enough ActorSpawnerBaseMachine with CanSpawnTank set to true, then no further tanks are spawned.
				ActorSpawnerBaseWithTank = nullptr;
			}
		}
		SpawnCountOfBatch++;
		SpawnCountTotalAllActorsOfClass++;
		
	}
}

// void APooledActorManager::SpawnSeriesOfBatches(FPooledClassSpawningData& PooledClassSpawningData, int32 TotalActorsToSpawnFraction)
// {
// 	int QuantityOfSeries = TotalActorsToSpawnFraction * PooledClassSpawningData.SpawnMultiplier;
// 	SpawnCountOfSeriesOfBatches = 0;
// 	int32 indexDecrementOfSpawner = 0;
// 	while (SpawnCountOfSeriesOfBatches < QuantityOfSeries)
// 	{
// 		
// 		//Find ideal spawn location
// 		FTransform Transform = GetTransformOfIdealActorSpawner(PooledClassSpawningData, indexDecrementOfSpawner);
//
// 		//return early if there are no available Spawning Machines. GetTransformOfIdealActorSpawner returns 0 if none available
// 		if (Transform.GetLocation() == FVector::Zero())
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Exit SpawnSeriesOfBatches early. No actorSpawners for class %s"), *PooledClassSpawningData.ActorClassToSpawn->GetName());
// 			return;
// 		}
// 		
// 		indexDecrementOfSpawner++;
// 		SpawnBatch(PooledClassSpawningData, indexDecrementOfSpawner, Transform);
// 		SpawnCountOfSeriesOfBatches++;
// 		///indexDecrementOfSpawner++;
// 	}
// }

// void APooledActorManager::TriggerSpawnOfSeriesBasedOnPriority()
// {
// 	//Calculate fraction of total actors. This number will be multiplied by the PooledClassSpawningData.Multiplier of a given class
// 	//This ensures the total number spawned is at or below the Max and 1st priroity spawns the full amount before the 2nd starts.
// 	int32 SumOfSpawnPriorities = 0;
// 	for (FPooledClassSpawningData& PooledClassSpawningData : _maPooledClassSpawningData)
// 	{
// 		SumOfSpawnPriorities += PooledClassSpawningData.SpawnPriority;
// 	}
// 	
// 	int32 TotalActorsToSpawnFraction = (_maxCharatersInLevel - _mCurrentCharatersInLevel) / SumOfSpawnPriorities;
// 	
// 	int32 indexOfPriority = 1;
// 	for (FPooledClassSpawningData& PooledClassSpawningData : _maPooledClassSpawningData)
// 	{
// 		if (PooledClassSpawningData.SpawnPriority == indexOfPriority
// 			&& PooledClassSpawningData.CharacterType != eActorTypeToSpawn::Tank)
// 		{
// 			SpawnSeriesOfBatches(PooledClassSpawningData, TotalActorsToSpawnFraction);
// 		}
// 		indexOfPriority++;
// 	}
// }

void APooledActorManager::TriggerSpawnOfBatches(FPooledClassSpawningData& PooledClassSpawningData)
{

	int MaxQuantityOfBatches = PooledClassSpawningData.NumberOfBatchesToSpawn;
	CurrentQuantityOfBatches = 0;
	int32 indexIncrumentOfSpawner = 0;
	while (CurrentQuantityOfBatches < MaxQuantityOfBatches)
	{
		
		//Find ideal spawn location
		//FTransform Transform = GetTransformOfIdealActorSpawner(PooledClassSpawningData, indexDecrementOfSpawner);
		FTransform Transform = GetTransformActorSpawner(PooledClassSpawningData, indexIncrumentOfSpawner);

		//return early if there are no available Spawning Machines. GetTransformOfIdealActorSpawner returns 0 if none available
		if (Transform.GetLocation() == FVector::Zero())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Exit SpawnSeriesOfBatches early. No actorSpawners for class %s"), *PooledClassSpawningData.ActorClassToSpawn->GetName());
			return;
		}
		
		indexIncrumentOfSpawner++;
		SpawnBatch(PooledClassSpawningData, Transform);
		CurrentQuantityOfBatches++;
		//SpawnCountOfSeriesOfBatches++;
		///indexDecrementOfSpawner++;
	}
}

void APooledActorManager::SetNumberOfBatches(FPooledClassSpawningData& PooledClassSpawningData, int32 TotalActorsToSpawn)
{
	int32 PortionOfTotalActorsToSpawnForClass = PooledClassSpawningData.EnemyRatio * TotalActorsToSpawn;

	if (PooledClassSpawningData.SpawnBatchSizeAtSingleLocation == 0 || PooledClassSpawningData.EnemyRatio == 0)
	{
		PooledClassSpawningData.NumberOfBatchesToSpawn = 0;
	}
	else
	{
		PooledClassSpawningData.NumberOfBatchesToSpawn = PortionOfTotalActorsToSpawnForClass / PooledClassSpawningData.SpawnBatchSizeAtSingleLocation;
	}
	
}

// void APooledActorManager::UpdateCurrentCharatersInLevel(int32 QuantityAddedOrSubtractedFromGamePlay)
// {
// 	
// 	//UE_LOG(LogTemp, Warning, TEXT("health depleted delegate triggered - UpdateCurrentCharatersInLevel"));
// 	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Black, TEXT("health depleted delegate triggered - UpdateCurrentCharatersInLevel"));
// 	_mCurrentCharatersInLevel += QuantityAddedOrSubtractedFromGamePlay;
// }

void APooledActorManager::CheckCharacterQuantityAndTriggerSpawn()
{
	// FString message = FString::FromInt(_mCurrentCharatersInLevel);
	// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Black, "Characters in Level");
	// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Black, message);
	if (_mCurrentCharatersInLevel < _mCharacterQuantityToTriggerSpawning)
	{
		int32 TotalActorsToSpawn = _maxCharatersInLevel - _mCurrentCharatersInLevel;
		SetNumberOfBatches(_mPooledClassSpawningDataFlusher, TotalActorsToSpawn);
		SetNumberOfBatches(_mPooledClassSpawningDataPinner, TotalActorsToSpawn);

		TriggerSpawnOfBatches(_mPooledClassSpawningDataFlusher);
		TriggerSpawnOfBatches(_mPooledClassSpawningDataPinner);

		
		//TriggerSpawnOfSeriesBasedOnPriority();
	}
}

int32 APooledActorManager::GetLowestStartPopulationVsBatchSize(FPooledClassSpawningData& PooledClassSpawningData)
{
	if (PooledClassSpawningData.PopulationGameStart < PooledClassSpawningData.SpawnBatchSizeAtSingleLocation)
	{
		//Batch size can't be 0, if population game start is greater than zero.
		return PooledClassSpawningData.PopulationGameStart;
	}
	else if (PooledClassSpawningData.SpawnBatchSizeAtSingleLocation <= 0 && PooledClassSpawningData.PopulationGameStart >= 1
		&& PooledClassSpawningData.CharacterType != eActorTypeToSpawn::Tank)
	{
		return PooledClassSpawningData.PopulationGameStart;
	}
	else if (PooledClassSpawningData.CharacterType != eActorTypeToSpawn::Tank)
	{
		return PooledClassSpawningData.SpawnBatchSizeAtSingleLocation;
	}
	else
	{
		return PooledClassSpawningData.SpawnBatchSizeAtSingleLocation;
	}
}


void APooledActorManager::TestTimerFunction()
{
	//UE_LOG(LogTemp, Warning, TEXT("Test TimerFunction fired"));
}





