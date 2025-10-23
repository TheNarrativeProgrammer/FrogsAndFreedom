// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/TokenData.h"
#include "AttackTokenRequesterable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackTokenRequesterable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IAttackTokenRequesterable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RequestAttackToken(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReserveAttackTokenAndStoreInMap(AActor* AttackTarget, int32 InIndexOfReservedPoint, FVector InLocationOfReservedPoint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnAttackTokens(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnAllAttackTokensForAllTargets();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector RequestAttackLocation(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasAttackToken(AActor* AttackTarget);

	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearTokenMap();
};
