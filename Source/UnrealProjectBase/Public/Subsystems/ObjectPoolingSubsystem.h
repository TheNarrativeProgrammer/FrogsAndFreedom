// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolingSubsystem.generated.h"

class AGameStateEventManagerBase;

class ACharacterBaseFrog;
/**
This takes in an Actor and creates a pool that can be drawn from. When an actor is in the pool,
their motion, tick, & world interaction are disabled. 
Drawing from the pool spawns an actor at a given location and enables their motion, tick and world interaction.

Because Maps can't take Arrays as values directly, the struct ActorArrayWrapper acts as a wrapper to allow a dictionary
with a AActor ptr as the key to have an array of all the AActors of the same type within a pool or within the game map.
 */


struct FActorQueueWrapper
{
	// GENERATED_BODY()
	// UPROPERTY(TRansient , BlueprintReadOnly)
	// TArray<AActor*> ActorsInPoolOrLevel;
	TQueue<AActor*> ActorsInPool;
};

struct FActorSetWrapper
{
	// GENERATED_BODY()
	// UPROPERTY(TRansient , BlueprintReadOnly)
	// TArray<AActor*> ActorsInPoolOrLevel;
	TSet<AActor*> ActorsInGamePlay;
};

UCLASS()
class UNREALPROJECTBASE_API UObjectPoolingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//OVERRIDE FUNCTIONS WITHIN USubSystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintReadWrite, Blueprintable)
	AGameStateEventManagerBase* GameStateEventManager;

	bool shouldStopPooling = false;

	UFUNCTION()
	void StopPoolingOnceKillAllFrogsObjectiveStarts();

	UFUNCTION()
	void BindDelegates();

	UFUNCTION()
	void UnBindDelegates();


	/*MAP OF ACTORS --> ptr to all Actors of a given class within the pool. Key is Class. Value is ptr to Actors of that class 
	The game level is also has a map with the same key value pairs.
	Actors can either be in their pool and deactivated or in the game level and activated.
	Each Class is tracked by two Maps, a Pool and a ActorsInGame.
	*/
	//TMap<TSubclassOf<AActor>, TQueue<AActor*>> _mdActorPoolsV2;
	
	TMap<TSubclassOf<AActor>, TUniquePtr<FActorQueueWrapper>> _mdActorPools;
	TMap<TSubclassOf<AActor>, FTransform> _mdPoolTransform;
	/*To make debugging easier, the pools of each class are given a different Location which is stored in the PoolsLocationMap
	*/
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Actors in level")
	TMap<TSubclassOf<AActor>, TUniquePtr<FActorSetWrapper>> _mdActorsInGamePlay;
	
	//TMap<TSubclassOf<AActor>, TSet<AActor*>> _mdActorsInGamePlayV2;
	
protected:
	UPROPERTY(visibleAnywhere)
	int32 _mCountActorsInPool;

	UPROPERTY(visibleAnywhere)
	int32 _mCountActorsInGame;

	/* SPAWNRATEOFTANKINFROGSPAWNCOUNT --> spawn a Tank Frog after X number of Pinner and Flush frogs have been spawned.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 _mSpawnRateOfTankInFrogSpawnCount;

public:
	/*PREWARM --> When game starts, this initializes a pool. Pass in all the Actor Classes you want a pool created for.
	 *Also, define the initial count for the pool and the location, with the location being outside of the gameplay area
	 *Null check -- valid class, if implments Poolable interface, Count and if it's a pawn (don't spawn controller)
	 */
	UFUNCTION(BlueprintCallable)
	void PreWarmPool(TSubclassOf<AActor> poolableClass, int32 count, FTransform poolTransform);

	/*SPAWNACTORFROMPOOL --> checks if the class passed in has a pool and if the pool size is greater than 0. If an actor
	 pool exist and there are actors in the pool, an actor is removed from the pool and spawned at the provided position
	 *If a pool doesn't exist or the pool is empty, a new Actor of that class is created and then spawned at the location
	 *In both cases, the Actor is added to the _mdActorsInGamePlay map.
	 *The actors tick, physics, collision, and interaction are ENABLED
	 */
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="poolableClass"))
	AActor* SpawnActorFromPool(TSubclassOf<AActor> poolableClass, const FTransform spawnTransform);

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="poolableClass"))
	AActor* SpawnNewActor(TSubclassOf<AActor> poolableClass, const FTransform spawnTransform);

	/*ACTORTORETURN --> removes an actor from the _mdActorsInGamePlay map and adds it to the _mdActorPools.
	 *The actors tick, physics, collision, and interaction are ENABLED
	 *The actors location is set based on the _mdPoolLocations map.
	 */
	UFUNCTION(BlueprintCallable)
	void ReturnToPool(AActor* actorToReturn);

	/*GETTOTALFROGSINGAMEPLAY --> returns the total number of Frogs in game play for ALL frog types. Looks at the _mdActorsInGamePlay for
	 *the classes that inherit from the CharacterBaseFrog and counts total.
	 * TPair <key, value> --> a key value pair within a map.
	 * For each key/value Pair in map, save it as a Tpair data type named KeyValuePair.
	 * & --> save as a reference, not copy
	 * .Value --> in the Tpair, access the value of the key value pair. The value is the ActorArrayWrapper.
	 * Acess the property ActorObjectPool within ActorArrayWrapper and count numbuer of actors in the array. Add count to totalFrogs
	 */ 
	UFUNCTION(BlueprintCallable)
	int32 GetTotalFrogsInGamePlayAllClasses();

	/*GETTOTALFROGSINGAMEPLAY --> returns the total number of Frogs in game play of a specific type. Looks at the _mdActorsInGamePlay for
	 *the class passed in.
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetTotalFrogsInGamePlaySingleClass(TSubclassOf<AActor> frogClass);

	UFUNCTION(BlueprintCallable)
	void OnLevelLoadReturnCurrentActorsInLevelToPool();

	UFUNCTION(BlueprintCallable)
	void OffloadPool();

	UFUNCTION(BlueprintCallable)
	void OnDeathClearPoolArrays();

	UFUNCTION(BlueprintCallable)
	void ReturnAllGamePlayActorsToPool();

	
	
	
};
