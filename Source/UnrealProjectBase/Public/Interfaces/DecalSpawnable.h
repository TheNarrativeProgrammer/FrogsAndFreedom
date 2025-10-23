// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DecalSpawnable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDecalSpawnable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IDecalSpawnable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetRotationAndLocationOfWall(bool& IsHit, FRotator& OutRotation, FVector& OutLocation, FVector InDirectionalVector);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnDecal(FRotator RotationOfSpawn, FVector LocationOfSpawn);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IfWallCloseSpawnDecal(FVector& DirectionalVector);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IterateOverNearWalls();
	
};
