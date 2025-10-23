// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetFrogABP.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetFrogABP : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IGetFrogABP
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetFrogABP();
};
