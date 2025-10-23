// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Interfaces/PatrolRouteable.h"
#include "CharacterBaseFrogPinner.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API ACharacterBaseFrogPinner : public ACharacterBaseFrog,
public ILaunchable,
public IPatrolRouteable
{
	GENERATED_BODY()

public:
	ACharacterBaseFrogPinner();
	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPoolActorActivated();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _mSpeedOfProjectile = 800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category="Route Variables")
	APatrolRouteBase* PatrolRouteBase;

	virtual void LaunchInDirection_Implementation(float SpeedMultiplier, FVector LocationOfForce) override;

	virtual void OnLaunchStartRotation_Implementation() override;

	virtual void OnLaunchTriggerFlailAnim_Implementation() override;

	//PatrolRouteable interface functions
	virtual APatrolRouteBase* GetPatrolRoute_Implementation() override;;

	virtual void SetPatrolRoute_Implementation(APatrolRouteBase* InPatrolRoute) override;

	virtual void IncrumentPatrolRouteIndex_Implementation() override;

	virtual int32 GetPatrolIndex_Implementation() override
	{
		return _mPatrolIndex;
	};

	UPROPERTY(BlueprintReadWrite)
	FVector _mPatrolPointQueryLocation;

	void SetPatrolPointQueryLocation_Implementation(FVector InPatrolPointQueryLocation) override;

	FVector GetPatrolPointQueryLocation_Implementation() override;

	protected:
	virtual void BeginPlay() override;
	
	
	
};
