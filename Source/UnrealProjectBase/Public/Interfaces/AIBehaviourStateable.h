// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "FrogAIable.h"
#include "UObject/Interface.h"
#include "AIBehaviourStateable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIBehaviourStateable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IAIBehaviourStateable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EAIStates GetState() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActorReturnToPoolUnbindFromBehaviourDelegates();

	
};
