// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawnerBase.h"
#include "../../Core/GameStateEventManagerBase.h"
#include "PooledActorManager.generated.h"

//Delegate signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMachineCountSigature, int, TotalMachineInLevel, int, RemainingMachinesInLevel);

UENUM()
enum eEnemyRatio : uint8
{
	Flusher_100___Pinner_0,
	Flusher_80____Pinner_20,
	Flusher_60____Pinner_40,
	Flusher_40____Pinner_60,
	Flusher_20____Pinner_80,
	Flusher_0_____Pinner_100,
};

/*ACTORSPAWNERBASEDATA --> data of each Machine and Vent in level
 * data initialized when level loads. It doesn't need to be set manually. 
 * DistanceThisToPlayer and DotProductToPlayer are updated when enemy count drops below threshhold
 * this struct is wrapper for data which is stored in list contains all active spawn locations in level.
 * overloaded operator compare if ptrs are same for ActorSpawnerBase
 */
USTRUCT(BlueprintType)
struct FActorSpawnerBaseData
{
	GENERATED_BODY()
	UPROPERTY()
	AActor* Actor = nullptr;
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	TEnumAsByte<eActorTypeToSpawn> CharacterSpawnType = eActorTypeToSpawn::Flusher;

	UPROPERTY()
	int32 DistanceThisToPlayer = 0;

	UPROPERTY()
	float DotProductToPlayer = 0.0f;

	UPROPERTY()
	bool CanSpawnActor = false;

	UPROPERTY()
	bool IsTankHere = false;

	UPROPERTY()
	bool CanTankSpawnHere = false;

	bool operator==(const FActorSpawnerBaseData& Other) const
	{
		return Actor == Other.Actor;
	}

	bool operator<(const FActorSpawnerBaseData& Other) const
	{
		return DistanceThisToPlayer == Other.DistanceThisToPlayer;
	}
};

/* POOLOFCLASSDATA --> data for pool initialization 
 * Defines Pool params:
 ** 1) class to make pool for
 ** 2) quantity of actors to spawn in pool on level load
 ** 3) location of pool in level.
 */
// USTRUCT(BlueprintType)
// struct FPoolOfClassData
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditInstanceOnly, BlueprintType, Blueprintable)
// 	TSubclassOf<AActor> ActorClassToSpawn;
// 	
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
// 	int32 PopulationInPool;
//
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
// 	FTransform Transform;
// };

/*POOLEDCLASSSPAWNINGDATA --> data for spawning (activating and relocating in game play) actors from pool
 * Defines spawning params:
 ** 1) quantity to spawn on level load
 ** 2) priority relative to other pooled classes. Depending on encounter, which class should be spawned first and be the highest quantity.
 ** 3) multipltier applied to default spawn rate of 1. If there are 3 pooled classes, by default they will spawn an equal amount
 *until TotalInLevel equals MaxInLevel. Giving one class a multiplier of 3 means 3 of that class will spawn at a time, then 1
 *of each of the other two classes will spawn, with this cycle repeating until the max is reached.
 *
 * Priority and multiplier work as a pair. Give a priority of 1 and a multiplier of 2-10 to the class that is the focus of an
 * encounter. This makes it spawn first in the line and spawn at a higher rate.
 * 4) BatchSizeAtSingleLocation defines how many characters of a given class spawn at the same location in the same spawn cycle
 */
USTRUCT(BlueprintType)
struct FPooledClassSpawningData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintType, Blueprintable)
	TSubclassOf<AActor> ActorClassToSpawn;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 PopulationGameStart = 1;

	UPROPERTY()
	float EnemyRatio = 0.0f;

	UPROPERTY()
	float NumberOfBatchesToSpawn = 0.0f;

	

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 SpawnPriority = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 SpawnMultiplier = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 SpawnBatchSizeAtSingleLocation = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 MimumumDotProductPlayerToSpawner = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int32 MaximumDistancePlayerToSpawner = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TEnumAsByte<eActorTypeToSpawn> CharacterType = eActorTypeToSpawn::Flusher;
};

/* BATCHSPAWNINGDATA --> because the timer is easiest to use when the functions have no parameters, instead of passing
 * this data in parameters, it's stored in a struct member variable that's updated in the calling function
 * each time a new batch of characters is spawned.
 * The function that is called then uses the member variable data.
 * For timers, try to call functions that take no parameters.
 */
USTRUCT(BlueprintType)
struct FBatchSpawningData
{
	GENERATED_BODY()
	UPROPERTY()
	FPooledClassSpawningData PooledClassSpawningData;

	UPROPERTY()
	FTransform SpawnTransform; 
};

/*POOLEDACTORMANAGER --> controls the spawning logic for characters.
 *
 * Important: Designer should fill in SpawnParameters and CharacterInfoNameOfCharacter in each instance. This data is meant
 * to vary in each encounter.
 * 
 * Finds all ActorSpawnerBase in current level. These are the possible spawn locations.
 * Register listener to health component of all ActorSpawnerBase. When health's deplected, remove it from array.
 * 
 * Registers listener to health component of all characters in pool. When death occurs, TotalKills is updated.
 *		When kills modulo of NumberOfKillTriggerSpawn is 0, characters are spawned until max is reached.
 *		
 * FMachineCountSigature - sends info on number of ActorSpawnerBase remaining in level
 *	
 */

class AActorSpawnerBaseFrogMachine;

UCLASS()
class UNREALPROJECTBASE_API APooledActorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APooledActorManager();

	//PROPERTTY - delegates
	/*MACHINECOUNTDELEGATE --> send info on the total ActorSpawnerBaseFrogMachines in level.
	 *called when the level loads and when the health of a machine reaches 0.
	 * send the total machines when level loaded and the remaining active mahcines currently in level (active mean health above 0) */
	UPROPERTY(Blueprintable, BlueprintAssignable, Category = "Delegates")
	FMachineCountSigature MachineCountDelegate;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Spawn Parameters")
	TEnumAsByte<eEnemyRatio> _mEnemyRatio;

	UFUNCTION(BlueprintCallable)
	void CallLevelStartFunctions();

	UPROPERTY()
	AGameStateEventManagerBase* GameStateEventManager;

	
	UFUNCTION()
	void SetEnemyRatios();

	UFUNCTION()
	void CheckForZeroDivisionAdjustSpawnBatchSize();

	UFUNCTION()
	void CheckForZeroBatchSizeWhenStartPopulationAboveZero(FPooledClassSpawningData& PooledClassSpawningData);

	//PROPERTIES - spawn parameters
	//once chracters start spawning, this controls the length of time between each spawn and the max that should be spawn
	/* SPAWNRATE --> length of time in seconds between each spawn. If set to 5, there is a 5 second delay between spawns*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Spawn Parameters")
	float _spawnRate;

	/*CHARACTERQUANTITYTOTRIGGERSPAWNING --> when the population of enemies reaches this value, spawning is triggered
	 * The population is calculated is agnostic of the enemies type. All enemies regardless of type count toward the population*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Spawn Parameters")
	int32 _mCharacterQuantityToTriggerSpawning;

	/*MAXCHARACTERSINLEVEL --> once the number of characters in the level reaches this amount, spawning stops*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Spawn Parameters")
	float _maxCharatersInLevel;

	/*CURRENTCHARACTERSINLEVEL --> population of enemies currently in the level.
	 *All enemies, regardless of type count, toward the population count .
	 *It's increased as enemies spawn from pool into level and decreased when an enemy's health is 0 and they move back to pool*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Spawn Parameters")
	float _mCurrentCharatersInLevel = 0;




	UFUNCTION()
	void IncreaseCurrentCharactersInLevel();

	UFUNCTION()
	void DecreaseCurrentCharactersInLevel();

	


	

	
	//PROPERTIES - Delegates and data sent in delegate
	//info for delegate to send. Once a machine's health is depleated, this data in sent
	/*TOTALMACHINESINLEVEL --> the total number of ActorSpawnerBaseFrogMachine's in the level when level loads
	 *1st param of the MachineCountDelegate*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ActorSpawnBaseMachine Count")
	int32 _mTotalMachineInLevel;

	/*REMAININGMACHINESINLEVEL --> the total number of active ActorSpawnerBaseFrogMachine's in level during gameplay.
	 *_maActorSpawnerBaseFrogMachines is an array tracking all active ActorSpawnerBaseFrogMachine's in the level,
	 *with active being a health greater than 0. When the health of a machine is 0, it's removed from the array.
	 * RemaiingMachinesInLevel counts the size of the array.
	 * 2nd param of the MachineCountDelegate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ActorSpawnBaseMachine Count")
	int32 _mRemainingMachineInLevel;

	

	//PROPERTIES - Character pool Data
	/*Data set by programmer. This is the number of actors of a given class that should be spawned in a pool.
	 *For each class that need a pool, an additional property should be added.*/
	// UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Pool Data")
	// FPoolOfClassData _mPoolOfClassDataFlusher;
	//
	// UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Pool Data")
	// FPoolOfClassData _mPoolOfClassDataPinner;
	//
	// UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Pool Data")
	// FPoolOfClassData _mPoolOfClassDataTank;

	
	//PROPERTIES - Character spawn Data
	/*Data set by Designer. This is spawning logic that varies based on encounters. See FPooledClassSpawningData for more info.
	 *For each class that spawns from pool, an additional property should be added.*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn Data")
	FPooledClassSpawningData _mPooledClassSpawningDataFlusher;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn Data")
	FPooledClassSpawningData _mPooledClassSpawningDataPinner;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn Data")
	FPooledClassSpawningData _mPooledClassSpawningDataTank;

	/*POOLCLASSSPAWNINGDATA --> array of 3 PoolClassSpawningData structs above.*/
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<FPooledClassSpawningData> _maPooledClassSpawningData;



	
	/*REPLACES ALL FActorSpawnerBaseData*/
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseFrogMachinesFlusher;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseFrogMachinesPinner;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseFrogMachinesTank;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<AActor*> _maActorSpawnerBaseVent;
	


	
	
	//PROPERTIES - array of ActorSpawnBases in level. 
	/*ACTORSPAWNERBASEFROGMACHINE --> array af all ActorSpawnerBaseFrogMachine. Child classes of ActorSpawnerBase*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ActorSpawner Machines in Level")
	TArray<FActorSpawnerBaseData> _maActorSpawnerBaseFrogMachines;

	/*ACTORSPAWNERBASEVENT --> array af all ActorSpawnerBaseVents. Child classes of ActorSpawnerBase*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ActorSpawner Vents in Level")
	TArray<FActorSpawnerBaseData> _maActorSpawnerBaseVents;

	
	//PROPERTIES - timer for spawn delay
	//todo implement timer. Currently, a while loop instantly spawns frogs. Timer should add 1ish second delay between spawns
	/*SPAWNTIMERHANDLE --> delay in frog spawning. When timer cycle ends, a new frog is spawned by calling SpawnActor().
	 * Actors continue being spawned until _mCurrentCharatersInLevel = _maxCharatersInLevel and the timer is then paused.*/
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	
	//PROPERTIES - spawning data
	UPROPERTY()
	FBatchSpawningData BatchSpawningDataCurrentBatch;

	
	//PROPERTIES --> persistent data for passing instant between functions
	/*ACTORSPAWNERBASEWITHTANK --> Persistent Data for ActorSpawnMachines. Used to pass reference of Tank to ActorSpawners
	 * The timer works best calling functions with no params, so ActorSpawners takes no params by design.
	 * To pass data, a persistent ptr is updated in the calling function and then used by the ActorSpawner() func later */
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	AActorSpawnerBaseFrogMachine* ActorSpawnerBaseWithTank;
	
	/*SPAWNCOUNTOFBATCH --> tracks the number of individual actors spawned in a given batch. Resets to 0 when batch size reached
	 * If batch size is 3, SpawnCountOfBatch starts at 0 and then iterates each time SpawnActor() is called. The batch is done
	 * when the count reaches 3, which then resets SpawnCountOfBatch to 0. This continues until the SpawnCountOfSeriesOfBatches
	 * count is reached.
	 * Whenever a new batch starts, the ActorSpawnBase is iterated through.
	 * The enemy types does not switch for different batches.*/
	UPROPERTY(VisibleDefaultsOnly)
	int32 SpawnCountOfBatch;

	UPROPERTY(VisibleDefaultsOnly)
	int32 SpawnCountTotalAllActorsOfClass;

	/*SPAWNCOUNTOFSERIESOFBATCHES --> tracks the number of batches spawned. Resets to zero when the series size is each and
	 * the enemy type switches. If a series size is 4, and the batch size is 2 for flushers will spawn 2 flusher at 
	 * the first ActorSpawnBase in the array (this being the first batch). Then 2 more flushers will spawn at the next ActorSpawnBase
	 * (this being the second batch). This continues until 4 batches are spawned.*/
	UPROPERTY(VisibleDefaultsOnly)
	int32 SpawnCountOfSeriesOfBatches;

	//Replace SpawnCountOfSeriesOfBatches
	UPROPERTY(VisibleDefaultsOnly)
	int32 CurrentQuantityOfBatches;

public:	
	virtual void Tick(float DeltaTime) override;

	//FUNCTIONS - On Begin play - populate arrays
	/*POPULATIONARRAYWITHACTORSOFCLASS --> populates the _maActorSpawnerBaseFrogMachines & _maActorSpawnerBaseVents
	 by finding all actors in level with that class and adding them to the array passed in*/
	UFUNCTION(BlueprintCallable)
	void PopulateArrayWithActorsOfClass(TSubclassOf<AActorSpawnerBase> classToSearchFor, TArray<FActorSpawnerBaseData>& inArrayToPopulate);

	//REPLACE PopulateArrayWithActorsOfClass
	// UFUNCTION(BlueprintCallable)
	// void OnInitializedAddActorSpawnerBaseToArray(AActor* Actor);

	//REPLACE RegisterListenersToHealthOfActorSpawners AND 


	//FUNCTIONS - On Begin play - RegisterListeners
	/*REGISTERLISTENERSTOHEALTHOFACTORSPAWNERS --> once array is populated, register listened to health component delegate
	 * When a characters health is zero:
	 * a) OnHealthDepleatedDecrementCountDelegate sends the number of actors to remove from the _mCurrentCharatersInLevel count.
	 * b) OnHealthDepletedDelegate calls CheckCharacterQuantityAndTriggerSpawn and the count is check to see if spawning should start*/
	// UFUNCTION(BlueprintCallable)
	// void RegisterListenersToHealthOfActorSpawners();

	// UFUNCTION(BlueprintCallable)
	// void RemoveListenersToHealthOfActorSpawners();

	/*REGISTERLISTENERSTOHEALTHOFCHARACTER --> access object pooling subsystem and register listener health of every actor in the pool
	 * When a characters health is zero, OnHealthDepletedSelfRefDelegate sends a refence to the machine. The machine is then
	 * removed from the array. This registers the delegate to call the removal event. */
	// UFUNCTION(BlueprintCallable)
	// void RegisterListenersToHealthOfCharacters(TSubclassOf<AActor> Actor);

	// UFUNCTION(BlueprintCallable)
	// void RemoveListenersToHealthOfCharacters(TSubclassOf<AActor> ActorClass);

	
	//FUNCTIONS - On Begin play - Delegates
	/*INITIALIZEDELEGATEVARIABLES --> Sets the initial values for variables send by MachineCountDelegate*/
	// UFUNCTION(BlueprintCallable)
	// void InitializeDelegateVariables();

	//REPLACES InitializeDelegateVariables. Incrumenets count each time machine is spawned
	// UFUNCTION(BlueprintCallable)
	// void IncreaseMachineCount();

	//REPLACES UpdateMachineCount
	// UFUNCTION(BlueprintCallable)
	// void DecreaseMachineCount();

	/*SENDINITIALBROADCAST --> send a first broadcast of totals for MachineCountDelegate. Use to initialize UI*/
	// UFUNCTION(BlueprintCallable)
	// void SendInitialBroadcast();

	//FUNCTIONS - On Begin play - object pooling
	/*INITIALIZEPOOL --> calls Prewarm pool, which is function within the ObjectPooling subsystem.
	 * Prewarm creates instances of enemies and puts them in the pool. The quantities and other specifics are defined in the
	 * FPoolOfClassData structs.*/
	// UFUNCTION(BlueprintCallable)
	// void InitalizePool(const FPoolOfClassData& poolOfClassData);
	

	//REPLACES RemoveInactiveMachineFromArray


	//FUNCTIONS - when machine count changes
	/*REMOVEINACTIVEMACHINEFROMARRAY --> called when ActorSpawnBaseMachine's health is depleated. Delegate send a ptr to
	machine with depleted health. The machine removed from the _maActorSpawnerBaseFrogMachines array and no longer is a
	possible spawn location for frogs. */
	UFUNCTION(BlueprintCallable)
	void RemoveInactiveMachineFromArray(AActor* ActorSpawnerBaseToRemove);

	/*UPDATEMACHINECOUNT --> called when ActorSpawnBaseMachine's health is depleated. Changes count of machine prior to the
	 *MachineCountDelegate being broadcast as the currentMachinesInLevel is data send by the delegate.*/
	// UFUNCTION(BlueprintCallable)
	// void UpdateMachineCount();

	/*BROADCASTMACHINECOUNT --> calls MachineCountDelegate after data updated*/
	// UFUNCTION(BlueprintCallable)
	// void BroadCastMachineCount();

	//FUNCTIONS - object pooling
	/*SPAWNACTOR --> access object pooling subsystem and call spawn actor. Removes an actor from the pool and spawns them
	 *in game play. Calls UpdateCurrentCharatersInLevel to update count in level.*/
	UFUNCTION(BlueprintCallable)
	void SpawnActor(TSubclassOf<AActor> ActorClassToSpawn, FTransform ActorSpawnerBaseTransform);

	//FUNCTIONS - get spawn location for actor
	/*GETTRANSFORMOFIDEALACTORSPAWNER --> when an actor is removed from the pool, this searches for all ActorSpawnerBaseFrogMachine
	 *that is the correct distance from the player and the corretly orientation (based on dot product) for pinner and flushers.
	 *For tanks, it checks if a tank is currently at this ActorSpawnerBaseFrogMachine, if it CanSpawnATank (bool set by designer)
	 *and if it's active (health above 0)
	 *
	 *When an ActorSpawnerBaseFrogMachine the passes the checks is found, it's add to an array.
	 *When a new batch starts, the same ActorSpawnerBaseFrogMachine index from the array is used for that batch. When the batch
	 *second batch starts, the index is incrumented and the next suitable ActorSpawnerBaseFrogMachine is used for the next batch.
	 *
	 *
	 *If no ActorSpawnerBaseFrogMachine match the criteria, then an ActorSpawnerBaseVent is selected.
	 *If no ActorSpawnerBaseFrogMachine or ActorSpawnerBaseVent are found, Fvector:Identity is used to flag a bad return.
	 */
	//Todo remove GetTransformofIdealActorSpawner. Replaced with function below it.
	UFUNCTION(BlueprintCallable)
	FTransform GetTransformOfIdealActorSpawner(FPooledClassSpawningData& PooledClassSpawningData, int32 indexDecrement);

	UFUNCTION(BlueprintCallable)
	FTransform GetTransformActorSpawner(FPooledClassSpawningData& PooledClassSpawningData, int32 indexDecrement);

	//FUNCTION - spawn initial amount
	UFUNCTION(BlueprintCallable)
	void OnGameStartPopulateWorld(FPooledClassSpawningData& PooledClassSpawningData);

	UFUNCTION(BlueprintCallable)
	void SpawnBatch(FPooledClassSpawningData& PooledClassSpawningData, FTransform SpawnTransform);

	UFUNCTION(BlueprintCallable)
	void SpawnActorFromBatch();

	// UFUNCTION(BlueprintCallable)
	// void SpawnSeriesOfBatches(FPooledClassSpawningData& PooledClassSpawningData, int32 TotalActorsToSpawnFraction);

	UFUNCTION()
	void TestTimerFunction();

	// UFUNCTION(BlueprintCallable)
	// void TriggerSpawnOfSeriesBasedOnPriority();

	UFUNCTION(BlueprintCallable)
	void TriggerSpawnOfBatches(FPooledClassSpawningData& PooledClassSpawningData);

	UFUNCTION()
	void SetNumberOfBatches(FPooledClassSpawningData& PooledClassSpawningData, int32 TotalActorsToSpawn);

	//todo remove this function. It's replaced by the increase and decrease functions below
	
	// UFUNCTION()
	// void UpdateCurrentCharatersInLevel(int32 QuantityAddedOrSubtractedFromGamePlay);
	

	

	UFUNCTION()
	void CheckCharacterQuantityAndTriggerSpawn();

	UFUNCTION()
	int32 GetLowestStartPopulationVsBatchSize(FPooledClassSpawningData& PooledClassSpawningData);

	

};





