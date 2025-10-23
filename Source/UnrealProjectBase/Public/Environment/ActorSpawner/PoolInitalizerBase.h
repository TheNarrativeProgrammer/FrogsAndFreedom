// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolInitalizerBase.generated.h"

/* POOLOFCLASSDATA --> data for pool initialization 
 * Defines Pool params:
 ** 1) class to make pool for
 ** 2) quantity of actors to spawn in pool on level load
 ** 3) location of pool in level.
 */
USTRUCT(BlueprintType)
struct FPoolOfClassData
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintType, Blueprintable)
	TSubclassOf<AActor> ActorClassToSpawn;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 PopulationInPool = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FTransform Transform = FTransform::Identity;
};

UCLASS()
class UNREALPROJECTBASE_API APoolInitalizerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolInitalizerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//PROPERTIES - Character pool Data
	/*Data set by programmer. This is the number of actors of a given class that should be spawned in a pool.
	 *For each class that need a pool, an additional property should be added.*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Pool Data")
	FPoolOfClassData _mPoolOfClassDataFlusher;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Pool Data")
	FPoolOfClassData _mPoolOfClassDataPinner;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Pool Data")
	FPoolOfClassData _mPoolOfClassDataTank;

	UFUNCTION(BlueprintCallable)
	void InitalizePool(const FPoolOfClassData& poolOfClassData);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
