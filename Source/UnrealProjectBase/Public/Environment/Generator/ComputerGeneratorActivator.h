// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "GeneratorBase.h"
#include "Components/BindComponents/DelegateBindComponentComputer.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "../../Core/GameStateEventManagerBase.h"
#include "ComputerGeneratorActivator.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGeneratorTimerFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecondElaspedSendTimeRemainingSignature, float, TimeRemaining);


UENUM()
enum eLeverState : uint8
{
	UpAndPullable,
	DownAndNotPullable,
};

USTRUCT(BlueprintType)
struct FGeneratorData
{
	GENERATED_BODY()
	UPROPERTY()
	AGeneratorBase* GeneratorBaseActor = nullptr;

	UPROPERTY()
	bool IsForceFieldActive = false;

	UPROPERTY()
	bool IsGeneratorHealthDepleted = false;
};




UCLASS()
class UNREALPROJECTBASE_API AComputerGeneratorActivator : public AActor,
public IInteractable,
public IDelegateBindBroadcastable
{
	GENERATED_BODY()
	
public:
	
	
	// Sets default values for this actor's properties
	AComputerGeneratorActivator();

	//DELEGATE - timer
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Delegate")
	FOnGeneratorTimerFinishedSignature OnGeneratorTimerFinishedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Delegate")
	FOnSecondElaspedSendTimeRemainingSignature OnSecondElaspedSendTimeRemainingDelegate;

	

	//FUNCTIONS - Interactable Interface 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseInteractVisual();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract_Implementation();

	UFUNCTION()
	void OnHealthDepletedOfGeneratorForcceFieldChangeComputerState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterStartBroadcastActorInitializationDelegates();

	

protected:

	//PROPERTIES - components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponentComputer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponentLever;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentComputer> DelegateBindComponentComputer;

	//PROPERTIES - timer
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FTimerHandle TimerHandleEncounterDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FTimerHandle TimerHandleDelegateBroadcastTimeUpdate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Timer")
	float _mTimeLengthUntilDoorOpens;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Timer")
	float _mTimeLengthBroadcastTimeElaspedDelegate = 1.0f;
	
	

	//PROPERTIES - generator
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
	bool _mIsGeneratorForceFieldStartingActive = false;

	

	//PROPERTIES - array of Generators in level
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Generators In Level Data")
	TArray<FGeneratorData> _maGeneratorData;

	//PROPERTIES - generator state
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generators State")
	TEnumAsByte<eLeverState> _meLeverState = eLeverState::UpAndPullable;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	UFUNCTION()
	void SetLeverState(TEnumAsByte<eLeverState> LeverState);
	

	

	UFUNCTION(BlueprintCallable)
	void OnLeverPullRestoreForceField();

	//FUNCTIONS - timerr
	UFUNCTION()
	void InitializeAndStartTimer();

	UFUNCTION(BlueprintCallable)
	void PauseTimer();

	UFUNCTION(BlueprintCallable)
	void RestartTimer();

	UFUNCTION()
	void OpenDoorToNextLevel();

	UFUNCTION()
	void BroadcastOnSecondElaspedSendTimeRemainingDelegate();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetElaspedTime() const;

	UFUNCTION(BlueprintCallable)
	float GetRemainingTime() const;

};
