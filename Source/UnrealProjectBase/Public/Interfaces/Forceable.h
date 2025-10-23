// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Forceable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UForceable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IForceable
{
	GENERATED_BODY()

	
public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnForceApplied(float force, float forceRadius, FVector sourceLocation);


};
