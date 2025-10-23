// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Interfaces/PatrolRouteable.h"
#include "CharacterBaseFrogFlush.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API ACharacterBaseFrogFlush : public ACharacterBaseFrog,
public ILaunchable,
public IPatrolRouteable
{
	GENERATED_BODY()

	public:
	ACharacterBaseFrogFlush();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPoolActorActivated();

	UFUNCTION(BlueprintCallable)
	void AdjustWalkSpeed(float WalkSpeedMultiplier);

	UFUNCTION(BlueprintCallable)
	void ResetWalkSpeedToDefault();

	virtual void LaunchInDirection_Implementation(float SpeedMultiplier, FVector LocationOfForce) override;

	virtual void OnLaunchStartRotation_Implementation() override;

	virtual void OnLaunchTriggerFlailAnim_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category="Route Variables")
	APatrolRouteBase* PatrolRouteBase;

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
	
	
	
};
