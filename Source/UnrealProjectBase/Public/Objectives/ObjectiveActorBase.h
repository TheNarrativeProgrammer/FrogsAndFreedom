// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "Interfaces/Objectiveable.h"
#include "ObjectiveActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveEventCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveEventCompletionReversedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveEventCompletedToggleCompletionTextSignature, bool, InIsCompletionTextVisible);
UCLASS(BlueprintType, Blueprintable)
class UNREALPROJECTBASE_API AObjectiveActorBase : public AActor,
public IGameplayTagAssetInterface,
public IObjectiveable
{
	GENERATED_BODY()
public:	
	AObjectiveActorBase();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	//Is this objective the first objective when level starts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Start Option")
	bool bStartWithEncounter = true;

	//Start and stop objective
	UPROPERTY(BlueprintReadOnly, Transient)
	uint8 _mHasObjectiveStarted : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Start Option", meta=(EditCondition="bStartWithEncounter==false", EditConditionHides))
	int32 _mReuqiredNumberOfPreObjectivesCompletedToStart = 1;
	
	UPROPERTY()
	int32 _mCountNumberOfPreObjectivesCompleted = 0;

	//Objective text - Started and completed
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI Display Text")
	FText _mObjectiveDisplayName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="UI Display Text On Completion")
	FText _mCompletionDisplayName;

	UPROPERTY(BlueprintReadOnly, Category="Completion Display Text Toggle")
	uint8 _mIsCompletionTextVisible : 1;

	UPROPERTY(BlueprintReadOnly, Category="Completion Display Text Toggle")
	uint8 _mIsFreed : 1;

	//Subobjectives
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Sub Objectives to Start On This Objective Complete")
	TSet<AObjectiveActorBase*> SubObjectives;

	UFUNCTION(BlueprintCallable)
	void StartSubObjectives() const;

	UFUNCTION(BlueprintCallable)
	void StopSubObjectives() const;

	//Enemy wave trigger
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="Wave"))
	FGameplayTagContainer _mWaveToTriggerOnObjectiveComplete;

	
	//Properties returned by IObjectiveable interface
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Influence Options")
	bool _mIsMustCompleteToStopSpawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Influence Options")
	bool _mIsMustCompleteToOpenDoor = false;

	UPROPERTY()
	bool _mIsAutoCompleteObjective = false;
	
	UFUNCTION(BlueprintCallable)
	void SetIsAutoCompleteObjective(bool InIsAutoCompleteObjective);

	//IObjectiveable Interface functions
	virtual bool GetIsMustCompleteToOpenDoor_Implementation() override {return _mIsMustCompleteToOpenDoor;}
	
	virtual bool GetIsMustCompleteToStopSpawn_Implementation() override {return _mIsMustCompleteToStopSpawn;};

	UFUNCTION()
	void IncreaseCountOfPreObjectivesCompleted();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsFreed(bool InIsFreed);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartObjective();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopObjective();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CompleteObjectiveBroadcast();

	UFUNCTION(BlueprintCallable)
	bool IsObjectiveStarted() const;

	UFUNCTION(BlueprintCallable)
	bool IsObjectivedFreed() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsObjectiveCompleted() const;

	virtual bool IsObjectiveCompleted_Implementation() const;

	UFUNCTION(BlueprintCallable,  meta = (Category="Wave"))
	void TriggerWave(const FGameplayTagContainer& InWaveToTriggerOnObjectiveComplete) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsCompletionTextVisible() const;

	virtual bool IsCompletionTextVisible_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsCompletionTextVisible(bool InIsCompletionTextVisilbe);

	UFUNCTION()
	void CompleteObjective();

	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UFUNCTION()
	void IfCompleteTriggerSubObjectivesAndWave();

	UPROPERTY()
	float mTimeDisplayCompletionText = 3.5f;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FObjectiveEventSignature OnObjectiveStarted;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FObjectiveEventCompletionReversedSignature ObjectiveEventCompletionReversedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FObjectiveEventCompletedToggleCompletionTextSignature ObjectiveEventCompletedToggleCompletionTextDelegate;
	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetObjectiveDisplayText() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetCompletionDisplayText() const;

	virtual FText GetObjectiveDisplayText_Implementation() const;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
