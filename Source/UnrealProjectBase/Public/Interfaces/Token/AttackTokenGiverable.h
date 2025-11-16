// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/TokenData.h"
#include "AttackTokenGiverable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackTokenGiverable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IAttackTokenGiverable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HandleResevereTokenRequest(const FRequesterGeneralTokenData& RequesterTokenData,
		const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleReturningToken(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector HandleAttackLocationRequest(int32 InIndexOfReservedPoint);
};
