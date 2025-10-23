// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveActorBase.h"
#include "ObjectiveActorSpawnersActive.generated.h"

/**
 * 
 */
class AActorSpawnerBaseFrogMachine;
class UDelegateBindingComponentObjSpawn;
UCLASS()
class UNREALPROJECTBASE_API AObjectiveActorSpawnersActive : public AObjectiveActorBase
{
	GENERATED_BODY()

public:
	AObjectiveActorSpawnersActive();

	virtual bool IsObjectiveCompleted_Implementation() const override;

	virtual FText GetObjectiveDisplayText_Implementation() const override;

	void StartObjective_Implementation() override;

	void CompleteObjectiveBroadcast_Implementation() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Actors Influencing Objectives")
	TSet<TObjectPtr<AActorSpawnerBaseFrogMachine>> _msActorSpawnerMachinesInLevel;

	UFUNCTION()
	void BroadcastUIUpdateEvent();

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDelegateBindingComponentObjSpawn> DelegateBindingComponentObjSpawn;
	
};
