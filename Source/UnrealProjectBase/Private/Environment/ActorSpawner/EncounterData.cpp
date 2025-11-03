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
#endif





	
	// for (auto& WaveData : EncounterWaveData)
	// {
	// 	EDataValidationResult ResultRatio = EDataValidationResult::Valid;
	// 	float CurrentRatio = 0.0f;
	// 	bool IsExclusiveSpawn = false;
	// 	for (auto& [EnemyTag, EncounterData] : WaveData.EncounterSpawnMap)
	// 	{
	// 		if (EncounterData.bIsExclusiveSpawn)
	// 		{
	// 			IsExclusiveSpawn = true;
	// 			continue;
	// 		}
	// 		if (EnemyTag.MatchesAnyExact(GameplayTagContainerFlusherPinner))
	// 		{
	// 			CurrentRatio += EncounterData.Ratio;
	// 		}
	// 		
	// 	}
	// 	if (IsExclusiveSpawn == false)
	// 	{
	// 	
	// 		ResultRatio = FMath::IsNearlyZero(CurrentRatio-1.0f) ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
	// 		if (ResultRatio == EDataValidationResult::Invalid)
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("When exclusive spawn is FALSE for all enemies, the sum of the ratio for the flusher and the pinner must be 1. The ratio for the tank is irrelevant."));
	// 			UE_LOG(LogTemp, Error, TEXT("Example: Flusher ratio: 1   Pinner ratio: 0"));
	// 			UE_LOG(LogTemp, Error, TEXT("Example: Flusher ratio: 0.5   Pinner ratio: 0.5"));
	// 			UE_LOG(LogTemp, Error, TEXT("Example: Flusher ratio: 0.8   Pinner ratio: 0.2"));
	// 		}
	// 	}
	// 	else
	// 	{
	// 	
	// 		ResultRatio = FMath::IsNearlyZero(CurrentRatio) ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
	// 		if (ResultRatio == EDataValidationResult::Invalid)
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("When exclusive spawn is TRUE for one enemy, the sum of the ratio for the flusher and the pinner must be 0. The ratio for the tank is irrelevant"));
	// 		}
	// 	}
	// 		
	// 	EDataValidationResult ResultIsExlusiveCount = EDataValidationResult::Valid;
	// 	int32 countIsExclusiveSpawn = 0;
	// 	for (auto& WaveData : EncounterWaveData)
	// 	{
	// 		for (auto& [EnemyTag, EncounterData] : WaveData.EncounterSpawnMap)
	// 		{
	// 			if (EncounterData.bIsExclusiveSpawn == true)
	// 			{
	// 				countIsExclusiveSpawn++;
	// 			}
	// 		}
	// 	
	// 	}
	// 	if (countIsExclusiveSpawn > 1)
	// 	{
	// 		ResultIsExlusiveCount = EDataValidationResult::Invalid;
	// 	}
	// }
	//
	//
	//
	//
	
	//
	// for (auto& WaveData : EncounterWaveData)
	// {
	// 	EDataValidationResult ResultTankBatch = EDataValidationResult::Valid;
	// 	if (WaveData.EncounterSpawnMap.Contains(FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Tank")))
	// 	{
	// 		FEncounterSpawnData EncounterDataTank = *WaveData.EncounterSpawnMap.Find(FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Tank"));
	// 		if (EncounterDataTank.InitialStartPopulation > 0 && EncounterDataTank.BatchSize == 0)
	// 		{
	// 			ResultTankBatch = EDataValidationResult::Invalid;
	// 			UE_LOG(LogTemp, Error, TEXT("If StartingPopulation is of Tank is above zero, BatchSize must be 1"));
	// 		}
	// 		if (EncounterDataTank.InitialStartPopulation > 0 && EncounterDataTank.BatchSize > 1)
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("If StartingPopulation is of Tank is above zero, BatchSize must be 1"));
	// 			ResultTankBatch = EDataValidationResult::Invalid;
	// 		}
	// 	}
	// 	EDataValidationResult Result = EDataValidationResult::Valid;
	// 	if (
	// 		ResultBatchSize == EDataValidationResult::Invalid
	// 		||ResultRatio == EDataValidationResult::Invalid
	// 		|| ResultTankBatch == EDataValidationResult::Invalid
	// 		|| ResultIsExlusiveCount == EDataValidationResult::Invalid)
	// 	{
	// 		Result = EDataValidationResult::Invalid;
	// 	}
	// 	
	// 	
	// }
	//
	//
	// return Result;
}


