// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.h"
#include "EnemyDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UEnemyDamageable : public UDamageable
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IEnemyDamageable
{
	GENERATED_BODY()
public:
	//FUNCTIONS - enemy attack locations
	 // UFUNCTION()
	 // virtual void PopulateEnemyAttackLocationsArray(USceneComponent* EnemyAttackLocation) = 0;
	 //
	 // UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 // TArray<FVector> GetEnemyAttackLocations();
	

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
