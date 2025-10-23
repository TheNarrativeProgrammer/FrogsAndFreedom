// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/BindComponents/DelegateBindingComponentGenTimer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "GeneratorTimerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGeneratorManagerTimerFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeneratorManagerSecondElaspedSendTimeRemainingSignature, float, TimeRemaining);

UCLASS()
class UNREALPROJECTBASE_API AGeneratorTimerManager : public AActor,
public IDelegateBindBroadcastable
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDelegateBindingComponentGenTimer> DelegateBindingComponentGenTimer;

	//PROPERTIES - timer
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FTimerHandle TimerHandleEncounterDurationGeneratorTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FTimerHandle TimerHandleDelegateBroadcastTimeUpdateGeneratorTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Timer")
	float _mTimeLengthObjectiveGeneratorDefence = 10.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Timer")
	float _mTimeLengthBroadcastTimeElaspedDelegate = 1.0f;

	UPROPERTY()
	bool _mIsTimerObjectiveComplete = false;
	
public:	
	AGeneratorTimerManager();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();
	
	UFUNCTION()
	void RestartTimer();

	UFUNCTION()
	void PauseTimer();

	UFUNCTION()
	void InitializeAndStartTimer();

	UFUNCTION()
	void TurnPowerOn();

	UFUNCTION()
	void TurnPowerOff();
	
	UFUNCTION()
	void BroadcastOnSecondElapsedSendTimeRemainingDelegate();

	UFUNCTION()
	void BroadcastObjectiveCompleteGeneratorTimer();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsTimerInitialized();
	
	UFUNCTION(BlueprintCallable)
	float GetElaspedTime() const;
	
	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const;

	UFUNCTION(BlueprintCallable)
	void OnGameOverClearTimer();

	

};
