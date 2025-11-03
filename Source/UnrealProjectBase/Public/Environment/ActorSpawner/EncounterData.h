// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Objectives/ObjectiveActorBase.h"
#include "EncounterData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EWaveTrigger : uint8
{
	EncounterStart UMETA(DisplayName = "EncounterStart"),
	FrogPopulation UMETA(DisplayName = "FrogPopulation"),
	Objective UMETA(DisplayName = "Objective"),
};

USTRUCT(BlueprintType)
struct FEncounterSpawnData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 QuantitySpawnedInWave = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 BatchSize = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsExclusiveSpawn = false;

	//How many batches should be spawned
	UPROPERTY()
	int32 BatchQuantity = 0;

	//although the tag is used as a key for the map, it's also used to filiter data and is necessary to have as a value.
	UPROPERTY()
	FGameplayTag GameplayTagFrog;
};

USTRUCT(BlueprintType)
struct FEncounterWaveData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(Categories="Wave"))
	FGameplayTag WaveTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(Categories="Character.Enemy.Type"))
	TMap<FGameplayTag, FEncounterSpawnData> EncounterSpawnMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn Options")
	float TimeDelayBetweenIndividualActorSpawn = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Method Of Starting Wave")
	TEnumAsByte<EWaveTrigger> WaveTrigger = EWaveTrigger::EncounterStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter",
		meta=(EditCondition="IsWaveTriggerFrogPopulation", EditConditionHides))
	int32 MinPopulationToTriggerSpawningNextWave = 0;

	UPROPERTY()
	bool HasWaveSpawned = false;

	UPROPERTY()
	bool CanWaveSpawn = false;
	
	bool IsWaveTriggerFrogPopulation() const {return WaveTrigger == FrogPopulation;}
	
};


UCLASS()
class UNREALPROJECTBASE_API UEncounterData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter")
	TArray<FEncounterWaveData> EncounterWaveData;

	UFUNCTION(BlueprintCallable, Category="Encounter", meta=(Categories="Wave"))
	FEncounterWaveData GetWaveData(const FGameplayTag WaveTag) const;

	UFUNCTION(Category="Encounter", meta=(Categories="Wave"))
	FEncounterWaveData& GetRefToWaveData(FGameplayTag WaveTag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter")
	int32 MaxPopulationToStopSpawningAllWaves;

	UFUNCTION()
	void SetCanWaveSpawn(const FGameplayTag WaveTag, const bool InCanWaveSpawn);

	UFUNCTION()
	void SetHasWaveSpawned(const FGameplayTag WaveTag, const bool InHasWaveSpawned);

	UFUNCTION()
	bool IsWaveInMap(const FGameplayTag WaveTag);

	

	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

#endif
	

	
	
};
