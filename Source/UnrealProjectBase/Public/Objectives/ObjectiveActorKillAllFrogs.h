// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/BindComponents/DelegateBindComponentObjKillAll.h"
#include "Objectives/ObjectiveActorBase.h"
#include "ObjectiveActorKillAllFrogs.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API AObjectiveActorKillAllFrogs : public AObjectiveActorBase
{
	GENERATED_BODY()
	
public:
	AObjectiveActorKillAllFrogs();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDelegateBindComponentObjKillAll> DelegateBindComponentObjKillAll;
	

	virtual void BeginPlay() override;

	bool IsObjectiveCompleted_Implementation() const override;

	void StartObjective_Implementation() override;

	void CompleteObjectiveBroadcast_Implementation() override;

	virtual FText GetObjectiveDisplayText_Implementation() const override;

	

	UPROPERTY()
	bool _mAreAllFrogsDead = false;

	UPROPERTY()
	int32 _mCountTotalEnemiesSpawned = 0;

	UPROPERTY()
	int32 _mCountEnemiesKilled = 0;

	UFUNCTION()
	void SetAreAllFrogsDead(bool InAreAllFrogsDead);

public:
	UFUNCTION()
	void IncreaseCountTotalEnemiesSpawned();
	
	UFUNCTION()
	void IncreaseCountEnemiesKilled();
	
};
