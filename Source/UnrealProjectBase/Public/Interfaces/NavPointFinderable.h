// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NavPointFinderable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNavPointFinderable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API INavPointFinderable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector FindReachablePointWhenOffNav();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetStateToPassive();
};
