// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Components/EncounterManagerComponent.h"
#include "Components/BindComponents/DelegateBindComponentEncountrTrg.h"
#include "EncounterTrigger.generated.h"

class UBoxComponent;
class UEncounterData;
class AObjectiveActorBase;
class EncounterManagerComponent;

UCLASS(Blueprintable, BlueprintType)
class UNREALPROJECTBASE_API AEncounterTrigger : public AActor
{
	GENERATED_BODY()
public:	
	AEncounterTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter")
	TArray<TObjectPtr<AObjectiveActorBase>> ObjectiveActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter")
	TObjectPtr<UEncounterData> EncounterData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentEncountrTrg> DelegateBindComponentEncountrTrg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> TriggerBox;
	
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentlyActiveWave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEncounterManagerComponent> mEncounterManagerComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter", meta = (ExposeFunctionCategories="Wave"))
	FGameplayTag DefaultWave;

	UPROPERTY()
	bool HasRegistered = false;

	UPROPERTY(BlueprintReadOnly)
	int32 _mCountTotalObjectives = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 _mCountCompletedObjectives = 0;

	UPROPERTY(BlueprintReadOnly)
	bool _mHasSpawnStopped = false;

	UPROPERTY(BlueprintReadOnly)
	bool _mIsDoorOpened = false;

	UPROPERTY(BlueprintReadOnly)
	int32 _mCountTotalObjectivesInfluencingSpawnStopping = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 _mCountCompletedObjectivesInfluencingSpawnStopping = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 _mCountTotalObjectivesInfluencingDoorOpening = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 _mCountCompletedObjectivesInfluencingDoorOpening = 0;

	UFUNCTION()
	void StartObjectives();

	UFUNCTION()
	void SetCountOfTotalObjectives();
	
	UFUNCTION()
	void IfAllObjectivesCompeleteThenBroadcast();

	UFUNCTION()
	void UpdateCountOfCompletedObjectives();

	UFUNCTION()
	void BroadcastObjectiveCompletionGameEvents();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnEncounterBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEncounterEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetObjectiveCountsToZero();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	UFUNCTION()
	void RegisterWithEncounterManager();

	UFUNCTION()
	void UnregisterFromEncounterManager();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AutoCompleteObjectives();

	UFUNCTION(BlueprintCallable)
	void OnSkipLevelUnregisterFromEncounterManager();

};
