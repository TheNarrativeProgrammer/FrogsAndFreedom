// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "../Core/GameStateEventManagerBase.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Environment/ActorSpawner/ActorSpawnerBaseFrogMachine.h"
#include "Environment/ActorSpawner/EncounterData.h"
#include "EncounterManagerComponent.generated.h"




class AEncounterTrigger;
class ACharacterBaseFrog;
class APatrolRouteBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class UNREALPROJECTBASE_API UEncounterManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEncounterManagerComponent();

	

	UPROPERTY(BlueprintReadWrite, Blueprintable)
	AGameStateEventManagerBase* GameStateEventManager;

	UPROPERTY(BlueprintReadOnly, Transient)
	AEncounterTrigger* CurrentlyActiveEncounterTrigger;

	UPROPERTY()
	bool _mIsAllObjectiveComplete = false;

	UPROPERTY()
	bool _mIsAllObjectiveInfluencingSpawningComplete = false;

	
	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Encounter", meta = (Categories = "Character.Enemy.Type"))
	TMap<FGameplayTag, TSoftClassPtr<ACharacterBaseFrog>> _mmEnemyData;

	UPROPERTY()
	TArray<FGameplayTag> _maEnemyGameplayTags;

	UPROPERTY()
	int32 _mEnemyTagIterator = 0;

	UPROPERTY()
	bool _mIsTankInWave = false;

	UPROPERTY()
	bool _mIsFlusherInWave = false;

	// UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Encounter", meta = (Categories = "Character.Enemy.Type"))
	// TMap<FGameplayTag, TSubclassOf<ACharacterBaseFrog>> _mmEnemyData;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag _mGameplayTagOfCurrentClassSpawning;

	UPROPERTY()
	FEncounterSpawnData EncounterSpawnDataCurrentBatch;

	UPROPERTY()
	APatrolRouteBase* PatrolRouteBaseOfCurrentBatch;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere,  meta=(Categories="Wave"))
	FGameplayTag CurrentActiveWave;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,  meta=(Categories="Wave"))
	FGameplayTag NextWaveInQue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,  meta=(Categories="Wave"))
	TMap<FGameplayTag, bool> _mmIsWaveObjectiveConditionMet;

	UFUNCTION(BlueprintCallable, meta=(Category="Wave"))
	void OnObjectiveCompleteStartSpawningWave(const FGameplayTag WaveToSpawn);

	UFUNCTION(BlueprintCallable, meta=(Category="Wave"))
	void AddWaveToObjectiveCompletionMap (const FGameplayTag WaveToSpawn);

	UFUNCTION(BlueprintCallable,  meta=(Categories="Wave"))
	void SetActiveWave(const FGameplayTag ActiveWave);

	UFUNCTION(BlueprintCallable)
	void SetNextWaveInQue();

	UFUNCTION()
	void SetCanWaveSpawnForEncounterData();

	UFUNCTION()
	void IterateToNextWaveIfTriggerIsFrogPopulationOrEncounter();

	UFUNCTION(BlueprintCallable)
	void RunEqsFindPatrolRoutes();
	
	
	UFUNCTION(BlueprintCallable)
	void RegisterEncounter(AEncounterTrigger* InEncounterTrigger);

	

	UFUNCTION(BlueprintCallable)
	void UnregisterEncounter(const AEncounterTrigger* InEncounterTrigger);

	UFUNCTION(BlueprintCallable)
	void AddEncounterActorSpawnersToArrays(AActor* Actor);

	UPROPERTY()
	bool IsEqsRunningAndPopulatingArray = false;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RunEqsFindActorSpawners();

	UFUNCTION(BlueprintCallable)
	void OnEqsCompleteStartFirstSpawn(bool InIsEqsSpawnerLocationComplete, bool InIsEqsRouteLocationComplete);

	UPROPERTY()
	bool _mIsEqsSpawnerLocationComplete = false;

	UPROPERTY()
	bool _mIsEqsRouteLocationComplete = false;

	UFUNCTION()
	bool GetIsSpawning() {return _misSpawning;}


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void EmptyArraysActorSpawnLocations();

	//Arrays of ActorSpawners
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseFrogMachinesFlusherPinner;

	// UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	// TArray<AActor*> _maActorSpawnerBaseFrogMachinesPinner;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseFrogMachinesTank;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseVent;

	//Property - count of enemies in level
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Enemy tracking data")
	int32 _mCurrentCharatersInLevel;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Enemy tracking data")
	int32 _mTotalCharatersSpawnedInAllWaves;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Enemy tracking data")
	int32 _mTotalCharatersSpawnedInCurrentWave = 0;

	UPROPERTY()
	FGameplayTag GameplayTagFlusher;

	UPROPERTY()
	FGameplayTag GameplayTagPinner;

	UPROPERTY()
	FGameplayTag GameplayTagTank;

	UPROPERTY()
	FGameplayTagContainer GameplayTagsContainerFlusherPinner;

	UPROPERTY()
	int32 BatchIterations;

	UPROPERTY()
	int32 QuantityOfActorsSpawnedFromBatch;

	UPROPERTY()
	int32 indexOfSpawnerArray;

	UPROPERTY()
	int32 _mIndexOfPatrolRoute;

	UPROPERTY()
	AActorSpawnerBaseFrogMachine* ActorSpawnerBaseFrogMachineMostRecentTankPlaced;

	

	UPROPERTY()
	float EncounterTimeBetweenSpawns;

	// UPROPERTY()
	// FTransform TransformSpawnLocationOfBatchFlusher;
	//
	// UPROPERTY()
	// FTransform TransformSpawnLocationOfBatchPinner;

	UFUNCTION()
	void AddForceToSpawnedActor(AActor* InSpawnedActor);

	UFUNCTION()
	void SetIsSpawningFromVent(bool InIsSpawningFromVent);

	UPROPERTY()
	bool _mIsSpawningFromVent = false;

	UPROPERTY()
	FTransform TransformSpawnLocationOfBatch;

	UPROPERTY()
	TSubclassOf<ACharacterBaseFrog> CharacterBaseFrogClassOfBatch;

public:
	UPROPERTY(BlueprintReadOnly)
	bool _misSpawning = false;

protected:
	UPROPERTY()
	int32 TotalActorsToSpawn;

	UPROPERTY()
	bool _mIsTankExculive = false;

	UPROPERTY()
	bool _mIsInitialSpawn = true;
	

	//radius of the EQS query based on the size of the encounter box. Take the x axis or y axis, whichever is larger
	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadOnly, Category="EQS")
	float _mRadiusOfEQSBaseOnEncounterSize;

	//b/c Extend of encounter is from the center of the box and not end to end, the X or Y extent should be multiplied by a factor to ensure the EQS radius encompasses the whole encounter. 
	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadOnly, Category="EQS")
	float _mEncounterSizeScaleFactor = 1.5;
	

	

	//Functions - remove machines from array on death
	UFUNCTION()
	void RemoveDeadMachineFromArray(AActor* Actor);

	//Functions - update count of Enemies in level on spawn and on death. On death, check if spawning should start.
	UFUNCTION()
	void IncreaseCurrentCharactersInLevel();

	UFUNCTION()
	void DecreaseCurrentCharactersInLevel();

	UFUNCTION()
	void AllObjectivesCompleted();

	UFUNCTION()
	void AllObjectiveInfluencingSpawningComplete();

	

	UFUNCTION()
	void ResetObjectiveComplete();

	//Function - Spawn batches
	UFUNCTION(BlueprintCallable)
	void StartSpawningIfConditionsMet();

	UFUNCTION(BlueprintCallable)
	void TriggerSpawnTimer();

	UFUNCTION()
	void OnEnemyDeathStartWaveIfWaveTriggerTypeIsPopulationBaseAndPopulationLow();

	UFUNCTION()
	void CheckSpawnValuesAndProcessIterations();

	UFUNCTION()
	void OnBatchCompeleteIterateToNextSpawnerWithWaveTag();

	UFUNCTION()
	void OnAllBatchSpawnedForEnemyTypeIterateToNextEnemyType();
	
	UFUNCTION()
	void OnWaveCompleteStopSpawning();

	UFUNCTION()
	void OnWaveCompleteResetData();

	UFUNCTION()
	void OnWaveCompleteIterateSpawnLocationAndWave();

	UFUNCTION()
	void OnWaveCompleteSetTimerTimeToNextWavesTime();

	UFUNCTION()
	void StartSpawnTimer();

	UFUNCTION()
	void StopSpawnTimer();

	UFUNCTION()
	void SpawnActorFromBatch();

	UFUNCTION()
	void OffLoadPool();

	UFUNCTION()
	void SpawnBatch(const FEncounterSpawnData& EncounterSpawnData, FTransform Transform);

	UFUNCTION()
	void SetNumberOfBatches(FEncounterSpawnData& EncounterSpawnData, int32 InTotalActorsToSpawn);

	UFUNCTION()
	void SetGameplayTagValue(FEncounterSpawnData& EncounterSpawnData, FGameplayTag GameplayTag);

	UFUNCTION()
	void IfExclusiveSpawnOverrideSpawnData();

	UFUNCTION()
	void ResetBoolsToDefault();

	UFUNCTION()
	void ResetMapOfObjectiveCompletionWaveRelease();

	

	UFUNCTION()
	void SetClassOfBatch(FEncounterSpawnData& EncounterSpawnData);

	UFUNCTION()
	void ResetBatchIterationAndSpawnerIndex();

	UFUNCTION()
	FTransform GetTransformActorSpawner(const FEncounterSpawnData& EncounterSpawnData, int32 InIndexOfSpawnerArray);

	UFUNCTION()
	void SetPatrolRouteCurrentBatch(FEncounterSpawnData& EncounterSpawnData,int32 InIndexOfPatrolRoute, APatrolRouteBase*& InPatrolRouteBaseOfCurrentBatch);

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
};
