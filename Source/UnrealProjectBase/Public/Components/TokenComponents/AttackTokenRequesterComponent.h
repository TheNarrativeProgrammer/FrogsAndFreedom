// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/TokenData.h"
#include "AttackTokenRequesterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECTBASE_API UAttackTokenRequesterComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAttackTokenRequesterComponent();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="TokenRequester Map of Tokens")
	TMap<AActor*, FRequesterGeneralTokenData> _mmTakenAttackTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TokenRequester Array Requested Tokens")
	TArray<int32> _maRequestedTokenIndexes;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RequestAttackToken(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReserveAttackTokenAndStoreInMap(AActor* AttackTarget, int32 InIndexOfReservedPoint,
		FVector InLocationOfReservedPoint);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnAttackTokens(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnAllAttackTokensForAllTargets();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasAttackTokens(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector RequestAttackLocation(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable)
	void ClearTokenMap();
	

	

	
		
};
