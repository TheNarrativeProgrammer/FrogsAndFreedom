// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PatrolRouteable.generated.h"

class APatrolRouteBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPatrolRouteable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class UNREALPROJECTBASE_API IPatrolRouteable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APatrolRouteBase* GetPatrolRoute();

	UFUNCTION(BlueprintNativeEvent)
	void SetPatrolRoute(APatrolRouteBase* InPatrolRoute);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrumentPatrolRouteIndex();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPatrolPointQueryLocation(FVector InPatrolPointQueryLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetPatrolPointQueryLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPatrolIndex();
	
};
