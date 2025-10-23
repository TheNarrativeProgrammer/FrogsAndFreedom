// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveActorBase.h"
#include "ObjectiveActorReachPoint.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()
class UNREALPROJECTBASE_API AObjectiveActorReachPoint : public AObjectiveActorBase
{
	GENERATED_BODY()

public:
	AObjectiveActorReachPoint();

	virtual void BeginPlay() override;

	bool IsObjectiveCompleted_Implementation() const override;

	void StartObjective_Implementation() override;

	void CompleteObjectiveBroadcast_Implementation() override;

	UPROPERTY()
	bool _mHasPlayerReachedPoint = false;

	UFUNCTION()
	void SetHasPlayerReachedPoint(bool InHasPlayerReachedPoint);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable, Category="Components")
	TObjectPtr<UBoxComponent> TriggerBox;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapObjectiveComplete(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
	
};
