// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveActorBase.h"
#include "ObjectiveActorTurnOnAllGenerator.generated.h"

/**
 * 
 */
class AGeneratorBase;
class UDelegateBindingComponentObjGenOn;
UCLASS()
class UNREALPROJECTBASE_API AObjectiveActorTurnOnAllGenerator
	: public AObjectiveActorBase
{
	GENERATED_BODY()
public:
	AObjectiveActorTurnOnAllGenerator();

	UPROPERTY(BlueprintReadOnly, Category="Components")
	UDelegateBindingComponentObjGenOn* DelegateBindingComponentObjGenOn;
	
protected:
	UPROPERTY(Transient)
	bool _IsFirstIterationComplete = false;

	UPROPERTY()
	bool _mIsTimerFinished = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI Display Text")
	FText _mObjectiveDisplayNameSecondIteration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actors Influencing Objective")
	TSet<TObjectPtr<AGeneratorBase>> _msGeneratorActorsInLevel;
public:
	virtual void BeginPlay() override;

	bool IsObjectiveCompleted_Implementation() const override;

	void StartObjective_Implementation() override;

	FText GetObjectiveDisplayText_Implementation() const override;
	

	UFUNCTION()
	void SetIsFirstIterationComplete ();

	UFUNCTION()
	void BroadcastUIUpdateEvent();

	UFUNCTION()
	void BroadcastCompletionReverseredEvent();
	
	void CompleteObjectiveBroadcast_Implementation() override;
	
	UFUNCTION()
	void SetIsTimerFinished(bool InIsTimerFinished);

	UFUNCTION()
	void OnTimerFinishedSetToTrue();
};
