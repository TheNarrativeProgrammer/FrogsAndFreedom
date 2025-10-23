// Copyright Peasant Pals 2025


#include "Components/EncounterManagerComponent.h"
#include "TimerManager.h"
#include "Environment/ActorSpawner/ActorSpawnerBaseVent.h"
#include "Environment/ActorSpawner/EncounterData.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Frogs/Enemies/CharacterBaseFrogTank.h"
#include "HealthComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Subsystems/ObjectPoolingSubsystem.h"
#include "GameplayTagContainer.h"
#include "Environment/ActorSpawner/EncounterTrigger.h"
#include "Components/PatrolRouteManagerComponent.h"
#include "Environment/Generator/GeneratorBase.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/PatrolRouteable.h"
#include "GameplayTagAssetInterface.h"


// Sets default values for this component's properties
UEncounterManagerComponent::UEncounterManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	
	// ...
}




void UEncounterManagerComponent::OnObjectiveCompleteStartSpawningWave(const FGameplayTag WaveToSpawn)
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(WaveToSpawn).HasWaveSpawned == false)
	{
		_mmIsWaveObjectiveConditionMet.Add(WaveToSpawn, true);
		if (_misSpawning == false)
		{
			RunEqsFindActorSpawners();
			PopulateArraysInPatrolRouteManager();
		}
	}
	
	
}

void UEncounterManagerComponent::SetActiveWave(const FGameplayTag ActiveWave)
{
	CurrentActiveWave = ActiveWave;
	SetNextWaveInQue();
}

void UEncounterManagerComponent::SetNextWaveInQue()
{

	if (CurrentActiveWave == FGameplayTag::RequestGameplayTag(FName("Wave.1")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.2"));
		}
	else if (CurrentActiveWave == FGameplayTag::RequestGameplayTag(FName("Wave.2")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.3"));
		}
	else if (CurrentActiveWave == FGameplayTag::RequestGameplayTag(FName("Wave.3")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.4"));
		}
	else if (CurrentActiveWave ==  FGameplayTag::RequestGameplayTag(FName("Wave.4")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.5"));
		}
	else if (CurrentActiveWave ==  FGameplayTag::RequestGameplayTag(FName("Wave.5")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.6"));
		}
	else if (CurrentActiveWave ==  FGameplayTag::RequestGameplayTag(FName("Wave.6")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.7"));
		}
	else if (CurrentActiveWave ==  FGameplayTag::RequestGameplayTag(FName("Wave.7")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.8"));
		}
	else if (CurrentActiveWave ==  FGameplayTag::RequestGameplayTag(FName("Wave.8")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.9"));
		}
	else if (CurrentActiveWave ==  FGameplayTag::RequestGameplayTag(FName("Wave.9")))
		{
			NextWaveInQue = FGameplayTag::RequestGameplayTag(FName("Wave.10"));
		}
}

void UEncounterManagerComponent::SetCanWaveSpawnForEncounterData()
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}

	if (CurrentlyActiveEncounterTrigger->EncounterData->IsWaveInMap(CurrentActiveWave) == false)
	{
		return;
	}
	
	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).WaveTrigger == EWaveTrigger::EncounterStart
		&& CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).CanWaveSpawn == false
		&& _misSpawning == false)
	{
		CurrentlyActiveEncounterTrigger->EncounterData->SetCanWaveSpawn(CurrentActiveWave, true);
	}

	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).WaveTrigger == EWaveTrigger::FrogPopulation
		&& CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).CanWaveSpawn == false
		&& _misSpawning == false)
	{
		if (_mCurrentCharatersInLevel <= CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).MinPopulationToTriggerSpawningNextWave)
		{
			CurrentlyActiveEncounterTrigger->EncounterData->SetCanWaveSpawn(CurrentActiveWave, true);
		}
	}

	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).WaveTrigger == EWaveTrigger::Objective
		&& CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).CanWaveSpawn == false
		&& _misSpawning == false)
	{
		 if (*_mmIsWaveObjectiveConditionMet.Find(CurrentActiveWave) == true)
		 {
		 	CurrentlyActiveEncounterTrigger->EncounterData->SetCanWaveSpawn(CurrentActiveWave, true);
		 }
	}
	
	
}

void UEncounterManagerComponent::IterateToNextWaveIfTriggerIsFrogPopulationOrEncounter()
{
		SetActiveWave(NextWaveInQue);
}

void UEncounterManagerComponent::PopulateArraysInPatrolRouteManager()
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		if (UPatrolRouteManagerComponent* PatrolRouteManagerComponent = Cast<UPatrolRouteManagerComponent>(GameMode->GetComponentByClass(UPatrolRouteManagerComponent::StaticClass())))
		{
			PatrolRouteManagerComponent->RunEqsFindPatrolRoutes();
		}
	}
}

void UEncounterManagerComponent::RegisterEncounter(AEncounterTrigger* InEncounterTrigger)
{
	if (!InEncounterTrigger) return;
	if (CurrentlyActiveEncounterTrigger != InEncounterTrigger)
	{
		CurrentlyActiveEncounterTrigger = InEncounterTrigger;
		SetActiveWave(CurrentlyActiveEncounterTrigger->DefaultWave);
		if (IsValid(CurrentlyActiveEncounterTrigger))
		{
			EncounterTimeBetweenSpawns = CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).TimeDelayBetweenIndividualActorSpawn;
			
			for (auto& EncounterWaveData : CurrentlyActiveEncounterTrigger->EncounterData->EncounterWaveData)
			{
				EncounterWaveData.CanWaveSpawn = false;
				EncounterWaveData.HasWaveSpawned = false;
			}
			ResetBoolsToDefault();
			ResetObjectiveComplete();
			PopulateArraysInPatrolRouteManager();
			RunEqsFindActorSpawners();
			
			if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>( UGameplayStatics::GetGameState(GetWorld())))
			{
				GameStateEventManager = GameState;
				if (GameStateEventManager)
				{
					GameStateEventManager->OnEncounterStartedDelegate.Broadcast();
					CurrentlyActiveEncounterTrigger->StartObjectives();
				}
			}
		}
	}
}


void UEncounterManagerComponent::UnregisterEncounter(const AEncounterTrigger* InEncounterTrigger)
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	if (CurrentlyActiveEncounterTrigger == InEncounterTrigger)
	{
		CurrentlyActiveEncounterTrigger = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		ResetBoolsToDefault();
		ResetObjectiveComplete();
		OffLoadPool();
	}
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>( UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
	}
	if (GameStateEventManager)
	{
		GameStateEventManager->FOnEncounterEndedDelegate.Broadcast();
	}
}

void UEncounterManagerComponent::AddEncounterActorSpawnersToArrays(AActor* Actor)
{
	if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(Actor))
	{
		FGameplayTagContainer GameplayTagContainer;
		if (IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(Actor))
		{
			TagAssetInterface->GetOwnedGameplayTags(GameplayTagContainer);
		}
		
		if (GameplayTagContainer.IsValid())
		{
			if (GameplayTagContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Environment.ActorSpawner.Machine"))))
			{
				_maActorSpawnerBaseFrogMachinesFlusherPinner.AddUnique(Actor);
			}
			if (GameplayTagContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Environment.ActorSpawner.Machine")))
				&& ActorSpawnerBaseFrogMachine->GetCanTankSpawnHere() == true)
			{
				_maActorSpawnerBaseFrogMachinesTank.AddUnique(Actor);
			}
		}
	}
	if (AActorSpawnerBaseVent* ActorSpawnerBaseVent = Cast<AActorSpawnerBaseVent>(Actor))
	{
		_maActorSpawnerBaseVent.AddUnique(Actor);
	}
}

void UEncounterManagerComponent::RunEqsFindActorSpawners_Implementation()
{
}


// Called when the game starts
void UEncounterManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	GameplayTagFlusher = FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Flusher");
	GameplayTagPinner = FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Pinner");
	GameplayTagTank = FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Tank");
	GameplayTagsContainerFlusherPinner.AddTag(GameplayTagFlusher);
	GameplayTagsContainerFlusherPinner.AddTag(GameplayTagPinner);

	//_maEnemyGameplayTags = {GameplayTagTank, GameplayTagFlusher, GameplayTagPinner};
	_maEnemyGameplayTags.Add(GameplayTagTank);
	_maEnemyGameplayTags.Add(GameplayTagFlusher);
	_maEnemyGameplayTags.Add(GameplayTagPinner);

	
	
}

void UEncounterManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.1")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.2")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.3")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.4")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.5")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.6")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.7")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.8")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.9")), false);
	_mmIsWaveObjectiveConditionMet.Add(FGameplayTag::RequestGameplayTag(FName("Wave.10")), false);
	
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>( UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
	}
	if (GameStateEventManager)
	{
		//Spawning Delegates
		GameStateEventManager->OnActorSpawnerBaseMachineDeathSelfRefDelegate.AddUniqueDynamic(this, &UEncounterManagerComponent::RemoveDeadMachineFromArray);
		//Enemy Delegates
		GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.AddUniqueDynamic(this, &UEncounterManagerComponent::IncreaseCurrentCharactersInLevel);
		GameStateEventManager->OnCharacterBaseFrogDeathDelegate.AddUniqueDynamic(this, &UEncounterManagerComponent::DecreaseCurrentCharactersInLevel);
		GameStateEventManager->OnCharacterBaseFrogDeathDelegate.AddUniqueDynamic(this, &UEncounterManagerComponent::OnEnemyDeathStartWaveIfWaveTriggerTypeIsPopulationBaseAndPopulationLow);
		GameStateEventManager->OnAllObectivesCompleteDelegate.AddUniqueDynamic(this, &UEncounterManagerComponent::AllObjectivesCompleted);
		GameStateEventManager->OnAllObjectiveInfluencingSpawningCompleteDelegate.AddUniqueDynamic(this, &UEncounterManagerComponent::AllObjectiveInfluencingSpawningComplete);
		//self ref to call initial spawn functions after others have initialized
		//GameStateEventManager->OnPooledActorManagerInitializedDelegate.AddUniqueDynamic(this, &APooledActorManager::CallLevelStartFunctions);
	}
	
}

void UEncounterManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	Super::EndPlay(EndPlayReason);
}

void UEncounterManagerComponent::EmptyArraysActorSpawnLocations()
{
	_maActorSpawnerBaseVent.Empty();
	_maActorSpawnerBaseFrogMachinesFlusherPinner.Empty();
	_maActorSpawnerBaseFrogMachinesTank.Empty();
}


void UEncounterManagerComponent::AddForceToSpawnedActor(AActor* InSpawnedActor)
{
	if (!InSpawnedActor)
	{
		return;
	}

	if (ACharacter* Character = Cast<ACharacter>(InSpawnedActor))
	{
			FVector LaunchDirection;
			
			if (_mIsSpawningFromVent == true)
			{
				LaunchDirection = FVector(0, 0, -1500.0f);	
			}
			else
			{
				FVector ForwardVector = InSpawnedActor->GetActorForwardVector();
				LaunchDirection = ForwardVector * 3000;
			}

			Character->LaunchCharacter(LaunchDirection, true, true);
	}
	
}

void UEncounterManagerComponent::SetIsSpawningFromVent(bool InIsSpawningFromVent)
{
	_mIsSpawningFromVent = InIsSpawningFromVent;
}

void UEncounterManagerComponent::RemoveDeadMachineFromArray(AActor* Actor)
{
	if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(Actor))
	{
		FGameplayTagContainer GameplayTagContainer = ActorSpawnerBaseFrogMachine->GameplayTagContainer;

		if (GameplayTagContainer.IsValid())
		{
			if (GameplayTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Environment.ActorSpawner.Machine"))))
			{
				if (_maActorSpawnerBaseFrogMachinesFlusherPinner.Contains(Actor))
				{
					_maActorSpawnerBaseFrogMachinesFlusherPinner.Remove(Actor);
				}

				if (_maActorSpawnerBaseFrogMachinesTank.Contains(Actor))
				{
					_maActorSpawnerBaseFrogMachinesTank.Remove(Actor);
				}
			}
		}
	}
}

void UEncounterManagerComponent::IncreaseCurrentCharactersInLevel()
{
	_mCurrentCharatersInLevel++;
}

void UEncounterManagerComponent::DecreaseCurrentCharactersInLevel()
{
	_mCurrentCharatersInLevel --;
}

void UEncounterManagerComponent::AllObjectivesCompleted()
{
	_mIsAllObjectiveComplete = true;
}

void UEncounterManagerComponent::AllObjectiveInfluencingSpawningComplete()
{
	_mIsAllObjectiveInfluencingSpawningComplete = true;
}

void UEncounterManagerComponent::ResetObjectiveComplete()
{
	_mIsAllObjectiveComplete = false;
	_mIsAllObjectiveInfluencingSpawningComplete = false;
}

void UEncounterManagerComponent::SetStartingSpawnValuesAndTriggerTimer()
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}

	//In tutorial, the only objective is to kill all frogs. This broadcast event when all frogs are dead.
	if (_mCurrentCharatersInLevel <= 0 && _mTotalCharatersSpawnedInAllWaves >= 1 && _mIsAllObjectiveInfluencingSpawningComplete == true)
	{
		GameStateEventManager->OnCharacterBaseFrogAllFrogsDeadDelegate.Broadcast();
	}

	//If all objectives that influence spawning are complete, then stop spawning. This is all objectives with
	//ObjectiveSpawnUntilComplete interface. The door and kill count objective don't have this interface.
	if (_mIsAllObjectiveComplete == true || _mIsAllObjectiveInfluencingSpawningComplete == true)
	{
		return;
	}

	if (_mTotalCharatersSpawnedInAllWaves >= CurrentlyActiveEncounterTrigger->EncounterData->MaxPopulationToStopSpawningAllWaves
		&& CurrentlyActiveEncounterTrigger->EncounterData->MaxPopulationToStopSpawningAllWaves >= 1)
	{
		return;
	}
	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).HasWaveSpawned)
	{
		return;
	}
	
	SetCanWaveSpawnForEncounterData();

	

	
	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).CanWaveSpawn == true
		&& CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).HasWaveSpawned == false
		&& _misSpawning == false)
	{
		_misSpawning = true;

		ResetBatchIterationAndSpawnerIndex();
		
		TotalActorsToSpawn = 0;
		int32 InitialPopulation  = 0;
		for (TPair<FGameplayTag, FEncounterSpawnData> Pair  : CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).EncounterSpawnMap)
		{
			InitialPopulation += Pair.Value.QuantitySpawnedInWave;
		}
		TotalActorsToSpawn = InitialPopulation;
		
		IfExclusiveSpawnOverrideSpawnData();

		//If excounter is Tank exclusive, then the tanks should spawn on level load and then nothing else should spawn.
		//After the initial spawn, this is called again when an enemy dies. This line exits early is the enocunter is tank exclusive and prevents more spawning
		if (_mIsTankExculive == true && _mIsInitialSpawn == false)
		{
			return;
		}
		
		
		
		
		
		for (auto& GameplayTagMMapKey : _maEnemyGameplayTags)
		{
			if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).EncounterSpawnMap.Contains(GameplayTagMMapKey) == false)
			{
				continue;
			}
			FEncounterSpawnData& EncounterSpawnData = *CurrentlyActiveEncounterTrigger->EncounterData->GetRefToWaveData(CurrentActiveWave).EncounterSpawnMap.Find(GameplayTagMMapKey);
			
			
			SetGameplayTagValue(EncounterSpawnData, GameplayTagMMapKey);
			
			SetNumberOfBatches(EncounterSpawnData, TotalActorsToSpawn);

			if (EncounterSpawnData.GameplayTagFrog.MatchesTagExact(GameplayTagTank) && EncounterSpawnData.BatchQuantity > 0 && _mIsInitialSpawn == true)
			{
				TransformSpawnLocationOfBatch = GetTransformActorSpawner(EncounterSpawnData, 0);
				EncounterSpawnDataCurrentBatch = EncounterSpawnData;
				_mEnemyTagIterator = 0;
				SetClassOfBatch(EncounterSpawnData);
				_mIsTankInWave = true;
			}
			else if (EncounterSpawnData.GameplayTagFrog.MatchesTagExact(GameplayTagFlusher) && EncounterSpawnData.BatchQuantity > 0
				&& _mIsTankInWave == false)
			{
				TransformSpawnLocationOfBatch = GetTransformActorSpawner(EncounterSpawnData, 0);
				SetPatrolRouteCurrentBatch(EncounterSpawnData, _mIndexOfPatrolRoute, PatrolRouteBaseOfCurrentBatch);
				EncounterSpawnDataCurrentBatch = EncounterSpawnData;
				_mEnemyTagIterator = 1;
				SetClassOfBatch(EncounterSpawnData);
				_mIsFlusherInWave = true;
			}
			else if (EncounterSpawnData.GameplayTagFrog.MatchesTagExact(GameplayTagPinner) && EncounterSpawnData.BatchQuantity > 0
				&& _mIsTankInWave == false && _mIsFlusherInWave == false)
			{
				TransformSpawnLocationOfBatch = GetTransformActorSpawner(EncounterSpawnData, 0);
				SetPatrolRouteCurrentBatch(EncounterSpawnData, _mIndexOfPatrolRoute, PatrolRouteBaseOfCurrentBatch);
				EncounterSpawnDataCurrentBatch = EncounterSpawnData;
				_mEnemyTagIterator = 2;
				SetClassOfBatch(EncounterSpawnData);
				
			}
			
			else
			{
				
			}
		}
		_mIsInitialSpawn = false;

		if (EncounterSpawnDataCurrentBatch.BatchQuantity > 0 && EncounterSpawnDataCurrentBatch.BatchSize > 0)
		{
			if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).WaveTag == FGameplayTag::RequestGameplayTag(FName("Wave.1")))
			{
				if (GameStateEventManager)
				{
					GameStateEventManager->OnWaveOneStartSpawningDelegate.Broadcast();
				}
			}
			
			StartSpawnTimer();
		}
		else
		{
			_misSpawning = false;
		}
	}
}

void UEncounterManagerComponent::OnEnemyDeathStartWaveIfWaveTriggerTypeIsPopulationBaseAndPopulationLow()
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}

	if (CurrentlyActiveEncounterTrigger->EncounterData->IsWaveInMap(CurrentActiveWave) == false)
	{
		return;
	}

	
	if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).WaveTrigger == EWaveTrigger::FrogPopulation
		&& CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).CanWaveSpawn == false
		&& _misSpawning == false)
	{
		if (_mCurrentCharatersInLevel <= CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).MinPopulationToTriggerSpawningNextWave)
		{
			SetStartingSpawnValuesAndTriggerTimer();
		}
	}
}

void UEncounterManagerComponent::OnEqsCompleteStartFirstSpawn(bool InIsEqsSpawnerLocationComplete, bool InIsEqsRouteLocationComplete)
{
	if (InIsEqsSpawnerLocationComplete == true)
	{
		_mIsEqsSpawnerLocationComplete = true;
	}
	if (InIsEqsRouteLocationComplete == true)
	{
		_mIsEqsRouteLocationComplete = true;
	}

	if (_mIsEqsSpawnerLocationComplete == true && _mIsEqsRouteLocationComplete == true)
	{
		SetStartingSpawnValuesAndTriggerTimer();
	}
	
}

void UEncounterManagerComponent::CheckSpawnValuesAndProcessIterations()
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	//Batch complete, iterate to next spawner
	if (QuantityOfActorsSpawnedFromBatch >= EncounterSpawnDataCurrentBatch.BatchSize)
	{
		OnBatchCompeleteIterateToNextSpawnerWithWaveTag();
	}

	//All Batches complete. Iterate to next enemy type.
	if (BatchIterations >= EncounterSpawnDataCurrentBatch.BatchQuantity)
	{
		// if (EncounterSpawnDataCurrentBatch.GameplayTagFrog.MatchesTag(GameplayTagTank))
		// {
		// 	return;
		// }
		OnAllBatchSpawnedForEnemyTypeIterateToNextEnemyType();
		
		if (_mEnemyTagIterator >= _maEnemyGameplayTags.Num()
			|| CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).HasWaveSpawned == true)
		{
			OnWaveCompleteStopSpawning();
			OnWaveCompleteResetData();
			OnWaveCompleteIterateSpawnLocationAndWave();
			return;
		}
		ResetBatchIterationAndSpawnerIndex();
		
		
		
		//If iteration gets to Batch that has zero quantity, skip to next iteration if not at end of array. If at end of array, stoptimer and reset.
		if (EncounterSpawnDataCurrentBatch.BatchQuantity <= 0)
		{
			if (_mEnemyTagIterator >= _maEnemyGameplayTags.Num() - 1)
			{
				OnWaveCompleteStopSpawning();
				OnWaveCompleteResetData();
				OnWaveCompleteIterateSpawnLocationAndWave();
				return;
			}
			else
			{
				OnAllBatchSpawnedForEnemyTypeIterateToNextEnemyType();
			}
		}
		
		SetClassOfBatch(EncounterSpawnDataCurrentBatch);
		SetPatrolRouteCurrentBatch(EncounterSpawnDataCurrentBatch, _mIndexOfPatrolRoute, PatrolRouteBaseOfCurrentBatch);
		TransformSpawnLocationOfBatch = GetTransformActorSpawner(EncounterSpawnDataCurrentBatch, indexOfSpawnerArray);
	}

	//All enemies spawned. Stop all spawning.
	if (_mTotalCharatersSpawnedInCurrentWave >= TotalActorsToSpawn )
	{
		OnWaveCompleteStopSpawning();
		OnWaveCompleteResetData();
		OnWaveCompleteIterateSpawnLocationAndWave();
	}
}

void UEncounterManagerComponent::OnBatchCompeleteIterateToNextSpawnerWithWaveTag()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == true)
	{
		QuantityOfActorsSpawnedFromBatch = 0;
		BatchIterations++;
		indexOfSpawnerArray++;
		_mIndexOfPatrolRoute++;
		if (BatchIterations < EncounterSpawnDataCurrentBatch.BatchQuantity)
		{
			TransformSpawnLocationOfBatch = GetTransformActorSpawner(EncounterSpawnDataCurrentBatch, indexOfSpawnerArray);
			if (EncounterSpawnDataCurrentBatch.GameplayTagFrog.MatchesAnyExact(GameplayTagsContainerFlusherPinner))
			{
				SetPatrolRouteCurrentBatch(EncounterSpawnDataCurrentBatch, _mIndexOfPatrolRoute, PatrolRouteBaseOfCurrentBatch);
			}
				
		}
	}
}

void UEncounterManagerComponent::OnAllBatchSpawnedForEnemyTypeIterateToNextEnemyType()
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	if (_mEnemyTagIterator == _maEnemyGameplayTags.Num() - 1)
	{
		CurrentlyActiveEncounterTrigger->EncounterData->SetHasWaveSpawned(CurrentActiveWave, true);
		return;
	}
	
	for (int i = _mEnemyTagIterator+1; i < _maEnemyGameplayTags.Num(); i++)
	{
		_mGameplayTagOfCurrentClassSpawning = _maEnemyGameplayTags[i];
		if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).EncounterSpawnMap.Contains(_mGameplayTagOfCurrentClassSpawning) == false)
		{
			if (i >= _maEnemyGameplayTags.Num() - 1)
			{
				CurrentlyActiveEncounterTrigger->EncounterData->SetHasWaveSpawned(CurrentActiveWave, true);
			}
			continue;
		}

		
		_mEnemyTagIterator = i;
		EncounterSpawnDataCurrentBatch = *CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).EncounterSpawnMap.Find(_mGameplayTagOfCurrentClassSpawning);
		break;
	}
}

void UEncounterManagerComponent::OnWaveCompleteStopSpawning()
{
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	CurrentlyActiveEncounterTrigger->EncounterData->SetHasWaveSpawned(CurrentActiveWave, true);
	StopSpawnTimer();
}

void UEncounterManagerComponent::OnWaveCompleteResetData()
{
	ResetBatchIterationAndSpawnerIndex();
	_mIsTankInWave = false;
	_mIsFlusherInWave = false;
	_mEnemyTagIterator = 0;
	_mTotalCharatersSpawnedInCurrentWave = 0;
	_mIsEqsSpawnerLocationComplete = false;
	_mIsEqsRouteLocationComplete = false;
	_misSpawning = false;
}

void UEncounterManagerComponent::OnWaveCompleteIterateSpawnLocationAndWave()
{
	
	if (CurrentlyActiveEncounterTrigger == nullptr || IsValid(CurrentlyActiveEncounterTrigger) == false || !CurrentlyActiveEncounterTrigger)
	{
		return;
	}

	if (CurrentlyActiveEncounterTrigger->EncounterData->IsWaveInMap(NextWaveInQue) == true)
	{
		if (CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(NextWaveInQue).WaveTrigger == EWaveTrigger::FrogPopulation
		|| CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(NextWaveInQue).WaveTrigger == EWaveTrigger::EncounterStart)
		{
			IterateToNextWaveIfTriggerIsFrogPopulationOrEncounter();
			OnWaveCompleteSetTimerTimeToNextWavesTime();
			RunEqsFindActorSpawners();
			PopulateArraysInPatrolRouteManager();
		}
	}
	
	
	
}

void UEncounterManagerComponent::OnWaveCompleteSetTimerTimeToNextWavesTime()
{
	EncounterTimeBetweenSpawns = CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).TimeDelayBetweenIndividualActorSpawn;
}

void UEncounterManagerComponent::StartSpawnTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UEncounterManagerComponent::SpawnActorFromBatch, EncounterTimeBetweenSpawns, true);
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerPaused(SpawnTimerHandle) == true)
		{
			GetWorld()->GetTimerManager().UnPauseTimer(SpawnTimerHandle);
		}
	}
}

void UEncounterManagerComponent::StopSpawnTimer()
{
if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == true)
	{
		if (GetWorld()->GetTimerManager().IsTimerPaused(SpawnTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().PauseTimer(SpawnTimerHandle);
		}
	}
}

void UEncounterManagerComponent::SpawnActorFromBatch()
{
	if (TransformSpawnLocationOfBatch.GetLocation() == FVector::Zero())
	{
		QuantityOfActorsSpawnedFromBatch++;
		CheckSpawnValuesAndProcessIterations();
		return;
	}

	if (!CurrentlyActiveEncounterTrigger)
	{
		return;
	}
	
	if (IsValid(CharacterBaseFrogClassOfBatch) == false || CharacterBaseFrogClassOfBatch == nullptr)
	{
		return;
	}
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		if (AActor* pooledActor = ObjectPoolingSubsystem->SpawnActorFromPool(CharacterBaseFrogClassOfBatch, TransformSpawnLocationOfBatch))
		{
			if (pooledActor == nullptr)
			{
				return;
			}
			if (EncounterSpawnDataCurrentBatch.BatchQuantity > 0)
			{
				if (pooledActor->Implements<UPatrolRouteable>())
				{
					IPatrolRouteable::Execute_SetPatrolRoute(pooledActor, PatrolRouteBaseOfCurrentBatch);
					GameStateEventManager->OnActorSpawnedSendTransformOfSpawnPointDelegate.Broadcast(TransformSpawnLocationOfBatch);
					AddForceToSpawnedActor(pooledActor);
				}
				//if spawned Actor is a tank, pass a ptr of the Tank to the ActorSpawner. This is used to determine if force field applied
				if (ACharacterBaseFrogTank* CharacterBaseFrogTank = Cast<ACharacterBaseFrogTank>(pooledActor))
				{
					//todo try to docouple OnSetTankSetForceFieldLocation(); this is temp solution
					ActorSpawnerBaseFrogMachineMostRecentTankPlaced->SetActorIDTankAtThisSpawner(CharacterBaseFrogTank->GetUniqueID());
					ActorSpawnerBaseFrogMachineMostRecentTankPlaced->OnSetTankSetForceFieldLocation();
					//ActorSpawnerBaseFrogMachineMostRecentTankPlaced->SetTankLocation(CharacterBaseFrogTank->GetTransform());
					CharacterBaseFrogTank->SendLocation();
					//reset persistent reference to the last ActorSpawnerBaseWithTank. There are null checks to see if this is valid
					//and it's updated only when a new tank spawns at a ActorSpawnerBaseMachine.
					//If there are not enough ActorSpawnerBaseMachine with CanSpawnTank set to true, then no further tanks are spawned.
					ActorSpawnerBaseFrogMachineMostRecentTankPlaced = nullptr;
				}
			}
			
		}
		_mTotalCharatersSpawnedInCurrentWave++;
		QuantityOfActorsSpawnedFromBatch++;
		_mTotalCharatersSpawnedInAllWaves++;
		CheckSpawnValuesAndProcessIterations();
	}
}

void UEncounterManagerComponent::OffLoadPool()
{
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		ObjectPoolingSubsystem->ReturnAllGamePlayActorsToPool();
		ObjectPoolingSubsystem->OffloadPool();
	}
}


void UEncounterManagerComponent::SpawnBatch(const FEncounterSpawnData& EncounterSpawnData, FTransform Transform)
{
	// BatchIterations++;
	// if (_mCurrentCharatersInLevel >= CurrentlyActiveEncounterTrigger->EncounterData->MaxPopulationToStopSpawning )
	// {
	// 	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == true)
	// 	{
	// 		GetWorld()->GetTimerManager().PauseTimer(SpawnTimerHandle);
	// 	}
	// 	return;
	// }
	// if (BatchIterations >= EncounterSpawnData.BatchQuantity)
	// {
	// 	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == true)
	// 	{
	// 		GetWorld()->GetTimerManager().PauseTimer(SpawnTimerHandle);
	// 	}
	// 	return;
	// }
	// if (QuantityOfActorsSpawnedFromBatch >= EncounterSpawnData.BatchSize)
	// {
	// 	if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == true)
	// 	{
	// 		GetWorld()->GetTimerManager().PauseTimer(SpawnTimerHandle);
	// 	}
	// }
	//
	// if (GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle) == false)
	// {
	// 	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UEncounterManagerComponent::SpawnEncounter, EncounterTimeBetweenSpawns, true);
	// }
	// else
	// {
	// 	if (GetWorld()->GetTimerManager().IsTimerPaused(SpawnTimerHandle) == true)
	// 	{
	// 		GetWorld()->GetTimerManager().UnPauseTimer(SpawnTimerHandle);
	// 	}
	// }
	
}

void UEncounterManagerComponent::SetNumberOfBatches(FEncounterSpawnData& EncounterSpawnData, int32 InTotalActorsToSpawn)
{
	
	if (EncounterSpawnData.GameplayTagFrog.MatchesAny(GameplayTagsContainerFlusherPinner))
	{
		if (EncounterSpawnData.BatchSize > 0 && EncounterSpawnData.QuantitySpawnedInWave > 0)
		{
			EncounterSpawnData.BatchQuantity = EncounterSpawnData.QuantitySpawnedInWave / EncounterSpawnData.BatchSize;
		}
		else
		{
			EncounterSpawnData.BatchQuantity = 0;
		}
	}
	else if (EncounterSpawnData.GameplayTagFrog.MatchesTagExact(GameplayTagTank))
	{
		if (_mIsInitialSpawn == true)
		{
			if (EncounterSpawnData.BatchSize > 0 && EncounterSpawnData.QuantitySpawnedInWave > 0){
				EncounterSpawnData.BatchQuantity = EncounterSpawnData.QuantitySpawnedInWave;
			}
			else
			{
				EncounterSpawnData.BatchQuantity = 0;
			}
		} else if (_mIsInitialSpawn == false)
		{
			EncounterSpawnData.BatchQuantity = 0;
		}
	}
}

void UEncounterManagerComponent::SetGameplayTagValue(FEncounterSpawnData& EncounterSpawnData, FGameplayTag GameplayTag)
{
	EncounterSpawnData.GameplayTagFrog = GameplayTag;
}

void UEncounterManagerComponent::IfExclusiveSpawnOverrideSpawnData()
{
	bool isExclusiveSpawnChecked = false;
	for (auto& [EnemyTag, EncounterData] : CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).EncounterSpawnMap)
	{
		if (EncounterData.bIsExclusiveSpawn == true  && isExclusiveSpawnChecked == false)
		{
			if (EnemyTag.MatchesTagExact(GameplayTagTank))
			{
				_mIsTankExculive = true;
			}
			isExclusiveSpawnChecked = true;
		}
	}

	if (isExclusiveSpawnChecked == true)
	{
		for (auto& [EnemyTag, EncounterData] : CurrentlyActiveEncounterTrigger->EncounterData->GetWaveData(CurrentActiveWave).EncounterSpawnMap)
		{
			if (EncounterData.bIsExclusiveSpawn == false)
			{
				EncounterData.BatchSize = 0;
				EncounterData.QuantitySpawnedInWave = 0;
			}
		}
	}
}

void UEncounterManagerComponent::ResetBoolsToDefault()
{
	_mIsTankExculive = false;
	_mIsInitialSpawn = true;
	_misSpawning = false;
	_mIsEqsSpawnerLocationComplete = false;
	_mIsEqsRouteLocationComplete = false;
	_mCurrentCharatersInLevel = 0;
	_maActorSpawnerBaseVent.Empty();
	_maActorSpawnerBaseFrogMachinesFlusherPinner.Empty();
	_maActorSpawnerBaseFrogMachinesTank.Empty();
	_mTotalCharatersSpawnedInAllWaves = 0;
	CharacterBaseFrogClassOfBatch = nullptr;
}


void UEncounterManagerComponent::SetClassOfBatch(FEncounterSpawnData& EncounterSpawnData)
{
	if (TSoftClassPtr<ACharacterBaseFrog>* CharacterBaseFrogSoftPtr = _mmEnemyData.Find(EncounterSpawnData.GameplayTagFrog))
	{
		UClass*  CharacterBaseFrogResolvedClass = CharacterBaseFrogSoftPtr->LoadSynchronous();
		if (CharacterBaseFrogResolvedClass)
		{
			CharacterBaseFrogClassOfBatch = TSubclassOf<ACharacterBaseFrog>(CharacterBaseFrogResolvedClass);
			
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("failed to load class from soft ptr"));
		}
		
	}
	
}

void UEncounterManagerComponent::ResetBatchIterationAndSpawnerIndex()
{
	QuantityOfActorsSpawnedFromBatch = 0;
	indexOfSpawnerArray = 0;
	_mIndexOfPatrolRoute = 0;
	BatchIterations = 0;
	CharacterBaseFrogClassOfBatch = nullptr;
}

FTransform UEncounterManagerComponent::GetTransformActorSpawner(const FEncounterSpawnData& EncounterSpawnData,int32 InIndexOfSpawnerArray)
{
	//_maActorSpawnerBaseFrogMachinesFlusher
	FTransform SpawnTransform = FTransform::Identity;
	
	if (EncounterSpawnData.GameplayTagFrog.MatchesAny(GameplayTagsContainerFlusherPinner))
	{
		if (InIndexOfSpawnerArray >= _maActorSpawnerBaseFrogMachinesFlusherPinner.Num() && _maActorSpawnerBaseFrogMachinesFlusherPinner.IsEmpty() == false)
		{
			InIndexOfSpawnerArray = InIndexOfSpawnerArray % _maActorSpawnerBaseFrogMachinesFlusherPinner.Num();
		}
		if (_maActorSpawnerBaseFrogMachinesFlusherPinner.IsEmpty() == false)
		{
			if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(_maActorSpawnerBaseFrogMachinesFlusherPinner[InIndexOfSpawnerArray]))
			{
				if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == true)
				{
					SetIsSpawningFromVent(false);
					SpawnTransform = ActorSpawnerBaseFrogMachine->GetTransformSpawneLocation();
				}
			}
		}
	}
	

	if (EncounterSpawnData.GameplayTagFrog.MatchesTag(GameplayTagTank))
	{
		if (InIndexOfSpawnerArray >= _maActorSpawnerBaseFrogMachinesTank.Num() && _maActorSpawnerBaseFrogMachinesTank.IsEmpty() == false)
		{
			InIndexOfSpawnerArray = InIndexOfSpawnerArray % _maActorSpawnerBaseFrogMachinesTank.Num();
		}

		if (_maActorSpawnerBaseFrogMachinesTank.IsEmpty()==false)
		{
			if (AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachine = Cast<AActorSpawnerBaseFrogMachine>(_maActorSpawnerBaseFrogMachinesTank[InIndexOfSpawnerArray]))
			{
				bool CanSpawnActor = ActorSpawnerBaseFrogMachine->GetCanSpawnActor();
				bool HasTankFrog = ActorSpawnerBaseFrogMachine->GetHasTankFrog();
				bool CanTankSpawnHere = ActorSpawnerBaseFrogMachine->GetCanTankSpawnHere();
				if (ActorSpawnerBaseFrogMachine->GetCanSpawnActor() == true
					&& ActorSpawnerBaseFrogMachine->GetHasTankFrog() == false
					&& ActorSpawnerBaseFrogMachine->GetCanTankSpawnHere() == true)
				{
					SpawnTransform = ActorSpawnerBaseFrogMachine->GetTransformSpawnLocationForTank();
					ActorSpawnerBaseFrogMachine->SetHasTankFrog(true);
					//save ptr of ActorSpawnerBase so it can be passed reference of the Tank spawned at it's location
					ActorSpawnerBaseFrogMachineMostRecentTankPlaced = ActorSpawnerBaseFrogMachine;
				}
			}
		}
	}
	
	//If no machines available and there are vents, then iterate through the vents array
	if (SpawnTransform.GetLocation() == FVector::Zero() && EncounterSpawnData.GameplayTagFrog.MatchesTagExact(GameplayTagTank) == false)
	{
		//if no Machines available and no vents in level, then retrn Tranform::identitty. This is used as flag to skip spawning more frogs of this type.
		if (_maActorSpawnerBaseVent.IsEmpty())
		{
			return SpawnTransform;
		}
		if (EncounterSpawnData.GameplayTagFrog.MatchesAny(GameplayTagsContainerFlusherPinner))
		{
			if (InIndexOfSpawnerArray >= _maActorSpawnerBaseVent.Num() && _maActorSpawnerBaseVent.IsEmpty() == false)
			{
				InIndexOfSpawnerArray = InIndexOfSpawnerArray % _maActorSpawnerBaseVent.Num();
			}
			if (_maActorSpawnerBaseVent.IsEmpty() == false)
			{
				if (AActorSpawnerBaseVent* ActorSpawnerBase = Cast<AActorSpawnerBaseVent>(_maActorSpawnerBaseVent[InIndexOfSpawnerArray]))
				{
					if (ActorSpawnerBase->GetCanSpawnActor() == true)
					{
						SetIsSpawningFromVent(true);
						SpawnTransform = ActorSpawnerBase->GetTransformSpawneLocation();
					}
				}
			}
		}
	}
	return SpawnTransform;
}

void UEncounterManagerComponent::SetPatrolRouteCurrentBatch(FEncounterSpawnData& EncounterSpawnData,
	int32 InIndexOfPatrolRoute, APatrolRouteBase*& InPatrolRouteBaseOfCurrentBatch)
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		if (UPatrolRouteManagerComponent* PatrolRouteManagerComponent = Cast<UPatrolRouteManagerComponent>(GameMode->GetComponentByClass(UPatrolRouteManagerComponent::StaticClass())))
		{
			PatrolRouteManagerComponent->SetPatrolRouteOfBatch(EncounterSpawnData, InIndexOfPatrolRoute, InPatrolRouteBaseOfCurrentBatch);
		}
	}
}


// Called every frame
void UEncounterManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

