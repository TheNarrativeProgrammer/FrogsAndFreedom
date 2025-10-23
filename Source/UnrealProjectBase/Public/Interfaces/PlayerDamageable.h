// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.h"
#include "PlayerDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerDamageable : public UDamageable
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IPlayerDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsShielded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsShielded(bool InIsShielded);
	

	
	
	
};
