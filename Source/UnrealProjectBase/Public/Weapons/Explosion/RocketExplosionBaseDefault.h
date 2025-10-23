// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Explosion/RocketExplosionBase.h"

#include "RocketExplosionBaseDefault.generated.h"

/**
 * 
 */
class USphereComponent;
UCLASS()
class UNREALPROJECTBASE_API ARocketExplosionBaseDefault : public ARocketExplosionBase
{
	GENERATED_BODY()

	public:
	ARocketExplosionBaseDefault();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Blueprintable, Category="Components")
	TObjectPtr<USphereComponent> SphereComponentDamage;

	UPROPERTY(EditDefaultsOnly, Blueprintable, Category="Components")
	TObjectPtr<USphereComponent> SphereComponentSense;

	UPROPERTY(EditDefaultsOnly, Blueprintable, Category="Components")
	TObjectPtr<USphereComponent> SphereComponentForce;

	virtual void SetRadiusOrWidithOfCollisionDamage_Implementation() override;

	virtual void SetRadiusOrWidithOfCollisionForce_Implementation() override;

	virtual void SetRadiusOrWidithOfCollisionSense_Implementation() override;

	virtual void SetLocationOfCollision_Implementation() override;

	virtual void IterateOverActorsCollisionDamage_Implementation() override;

	virtual TArray<AActor*> GetOverlappingActorsInCollision_Implementation(bool& InReturnValue, FVector Location, float RadiusOrWidth, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes) override;
};
