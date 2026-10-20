// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/EncounterData.h"


FEncounterWaveData UEncounterData::GetWaveData(const FGameplayTag WaveTag) const
{
	for (auto& WaveData : EncounterWaveData)
	{
		if (WaveData.WaveTag == WaveTag)
		{
			return WaveData;
		}
	}
	return FEncounterWaveData();
}

FEncounterWaveData& UEncounterData::GetRefToWaveData(FGameplayTag WaveTag)
{
	for (auto& WaveData : EncounterWaveData)
	{
		if (WaveData.WaveTag == WaveTag)
		{
			return WaveData;
		}
	}
	return EncounterWaveData[0];
}


void UEncounterData::SetCanWaveSpawn(const FGameplayTag WaveTag, const bool InCanWaveSpawn)
{
	for (auto& WaveData : EncounterWaveData)
	{
		if (WaveData.WaveTag == WaveTag)
		{
			WaveData.CanWaveSpawn = InCanWaveSpawn;
		}
	}
}

void UEncounterData::SetHasWaveSpawned(const FGameplayTag WaveTag, const bool InHasWaveSpawned)
{
	for (auto& WaveData : EncounterWaveData)
	{
		if (WaveData.WaveTag == WaveTag)
		{
			WaveData.HasWaveSpawned = InHasWaveSpawned;
		}
	}
}

bool UEncounterData::IsWaveInMap(const FGameplayTag WaveTag)
{
	for (auto& WaveData : EncounterWaveData)
	{
		if (WaveData.WaveTag == WaveTag)
		{
			return true;
		}
	}
	return false;
}
#if WITH_EDITOR
EDataValidationResult UEncounterData::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult ResultSuper =  Super::IsDataValid(Context);
	for (auto& WaveData : EncounterWaveData)
	{
		EDataValidationResult ResultBatchSize = EDataValidationResult::Valid;
		for (auto& [EnemyTag, EncounterData] : WaveData.EncounterSpawnMap)
		{
			if (EncounterData.BatchSize <= 0 || EncounterData.QuantitySpawnedInWave <= 0)
			{
				ResultBatchSize = EDataValidationResult::Invalid;
				UE_LOG(LogTemp, Error, TEXT("BatchSize and QuantitySpawnedInWave can't be less than 1"));
			}
		}
	}
	return ResultSuper;

}

void UEncounterData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	int previousTotal = 0;
	for (FEncounterWaveData& WaveData : EncounterWaveData)
	{
		WaveData.UpdateDerivedFlags();
		WaveData.totalSpawnedUpToCurrentWave = 0;
		
		for (auto keyValue : WaveData.EncounterSpawnMap)
		{
			WaveData.totalSpawnedUpToCurrentWave += keyValue.Value.QuantitySpawnedInWave;
		}
		WaveData.totalSpawnedUpToCurrentWave += previousTotal;
		previousTotal = WaveData.totalSpawnedUpToCurrentWave;
		
		
	}
}
#endif

