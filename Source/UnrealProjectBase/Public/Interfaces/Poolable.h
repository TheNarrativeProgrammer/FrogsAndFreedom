// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	//NOTE: ADD FUNCTION TO THE "I" CLASS. Do NOT add to the "u" class
public:
	/*ONPOOLACTORACTIVATED --> add code that fires when Actor is removed from Pool and spawned in Gameplay
	 *TODO body of this function currently isn't implemeneted. It's here incase it's needed
	 *This is called in ObjectPoolingSubsystem in the PreWarm and ReturnToPool func
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPoolActorActivated();

	/*ONPOOLACTORDEACTIVATED --> add code that fires when a) pool is prewarmed & b) Actor returns to pool
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPoolActorDeactivated();
};
