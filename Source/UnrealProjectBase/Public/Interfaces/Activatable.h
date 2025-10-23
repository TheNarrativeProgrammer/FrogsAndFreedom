// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activatable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActivatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IActivatable
{
	GENERATED_BODY()

public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActivated();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeactivated();
};
