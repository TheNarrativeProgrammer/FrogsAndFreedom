// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Explosion/RocketExplosionBase.h"

#include "RocketExplosionBasePogo.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class UNREALPROJECTBASE_API ARocketExplosionBasePogo : public ARocketExplosionBase
{
	GENERATED_BODY()

public:
	ARocketExplosionBasePogo();

	UPROPERTY(EditDefaultsOnly, Blueprintable, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponentDamage;

	UPROPERTY(EditDefaultsOnly, Blueprintable, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponentForce;

	UPROPERTY(EditDefaultsOnly, Blueprintable, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponentSense;
	

	UPROPERTY()
	float _mBoxHeightZaxis;
	
	virtual void SetRadiusOrWidithOfCollisionDamage_Implementation() override;

	virtual void SetRadiusOrWidithOfCollisionForce_Implementation() override;

	virtual void SetRadiusOrWidithOfCollisionSense_Implementation() override;

	virtual void SetLocationOfCollision_Implementation() override;

	virtual void IterateOverActorsCollisionDamage_Implementation() override;

	virtual TArray<AActor*> GetOverlappingActorsInCollision_Implementation(bool& InReturnValue, FVector Location, float RadiusOrWidth, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes) override;

	virtual void BeginPlay() override;

	
	
	
};
