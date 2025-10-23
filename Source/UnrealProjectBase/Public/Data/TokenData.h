// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "TokenData.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct UNREALPROJECTBASE_API FRequesterGeneralTokenData
{
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CountTokensTaken = 0;
	
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TokensRequiredForAction = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 IndexOfReservedPoint = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector LocationOfReservedPoint = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FRequesterReservationData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 IndexOfReservedPoint = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector LocationOfReservedPoint = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct UNREALPROJECTBASE_API FGiverAttackTokenData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponentAttackLocation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsTokenAvailable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TokensAvailableInSlot = 1;
};

class UNREALPROJECTBASE_API TokenData
{
public:
	TokenData();
	~TokenData();
};
