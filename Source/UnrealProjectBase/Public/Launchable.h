// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Launchable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULaunchable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API ILaunchable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LaunchInDirection(float SpeedMultiplier, FVector LocationOfForce);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLaunchStartRotation();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLaunchTriggerFlailAnim();

	
};
