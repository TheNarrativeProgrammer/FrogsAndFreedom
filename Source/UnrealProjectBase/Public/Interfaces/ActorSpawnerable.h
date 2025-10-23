// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorSpawnerable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorSpawnerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IActorSpawnerable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetActorIDTankAtThisSpawner();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetActorIDOfSpawner();
};
