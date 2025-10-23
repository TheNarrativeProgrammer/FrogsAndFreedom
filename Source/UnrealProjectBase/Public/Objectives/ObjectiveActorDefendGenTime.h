// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveActorBase.h"
#include "Components/BindComponents/DelegateBindingComponentObjTimer.h"
#include "ObjectiveActorDefendGenTime.generated.h"

/**
 * 
 */
class AGeneratorTimerManager;
UCLASS()
class UNREALPROJECTBASE_API AObjectiveActorDefendGenTime : public AObjectiveActorBase
{
	GENERATED_BODY()
public:
	AObjectiveActorDefendGenTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDelegateBindingComponentObjTimer> DelegateBindingComponentGenTimer;
protected:
	UPROPERTY(BlueprintReadOnly)
	bool _mIsTimerFinished = false;

	UPROPERTY(BlueprintReadOnly)
	float _mTimeRemaining = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Generator Time Manager")
	TObjectPtr<AGeneratorTimerManager> GeneratorTimerManager;
public:
	bool IsObjectiveCompleted_Implementation() const override;

	void StartObjective_Implementation() override;

	void StopObjective_Implementation() override;
	
	FText GetObjectiveDisplayText_Implementation() const override;
	
	void BroadcastUIUpdateEvent();
	
	void CompleteObjectiveBroadcast_Implementation() override;
	
	UFUNCTION()
	void SetIsTimerFinished();

	UFUNCTION()
	void UpdateTimeRemainingAndBroadcastToUI(float InTimeRemaning);
	
};
