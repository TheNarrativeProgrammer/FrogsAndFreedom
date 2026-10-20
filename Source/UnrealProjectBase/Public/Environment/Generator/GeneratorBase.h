// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ForceFieldBase.h"
#include "Components/SceneComponent.h"
#include "HealthComponent.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Token/AttackTokenGiverable.h"
#include "../../Interfaces/EnemyDamageable.h"
#include "Components/TokenComponents/AttackTokenGiverComponent.h"
#include "Components/BindComponents/DelegateBindComponentGenerator.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Components/BoxComponent.h"
#include "GeneratorBase.generated.h"

//todo Remove this delegate and its calls and listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepetedForceFieldSignature);

class UHealthComponent;
class AComputerGeneratorActivator;
UCLASS(Blueprintable, BlueprintType)
class UNREALPROJECTBASE_API AGeneratorBase : public AActor,
public IDamageable,
public IDelegateBindBroadcastable,
public IEnemyDamageable,
public IGameplayTagAssetInterface,
public IGenericTeamAgentInterface,
public IInteractable,
public IAttackTokenGiverable
{
	GENERATED_BODY()
	
public:	
	AGeneratorBase();

//PROPERTIES

	//Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnHealthDepetedForceFieldSignature OnHealthDepetedForceFieldDelegate;

	//Gameplay tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayTagContainer")
	FGameplayTagContainer GameplayTagContainer;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<AForceFieldBase> mForceFieldPtr = nullptr;
	
	//components
public:
	UPROPERTY()
	TObjectPtr<UPawnNoiseEmitterComponent> NoiseEmitterComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentGenerator> DelegateBindComponentGenerator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentJumpOffLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackTokenGiverComponent> AttackTokenGiverableComponent;
	

	//Force field
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Encounter Options")
	bool _mIsShieldActiveOnEncounterStart = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ForceFieldID")
	int32 _mForceFieldID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Field Class")
	TSubclassOf<AForceFieldBase> ForceFieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Field Class")
	bool _mIsForceFieldActive;

	UPROPERTY()
	FTimerHandle GeneratorSoundTimerHandle;

	UPROPERTY()
	float TimeBetweenNoises = 5.0f;

public:
	UFUNCTION()
	void CheckForceFieldIDMatchesThisOwner(int32 InForceField);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterStartBroadcastActorInitializationDelegates();

	UFUNCTION(BlueprintCallable)
	void SetSoundTimer();

private:
	//PROPERTIES - team ID for GenericTeamAgentInterface
	UPROPERTY(EditDefaultsOnly)
	FGenericTeamId TeamId;


//INTERFACES
public:
	//IGameplayerTagAssetInterface
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;
	
	//IGenericTeamAgentInterface. IdNumber is public and is used to change the TeamId variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 IdNumber;

	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }

	//Interactable interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseInteractVisual();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractMessageOn(FRotator PlayerRotator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractMessageOff();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract_Implementation();
	
	//IAttackTokenGiverable
	virtual void HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint) override;

	virtual bool HandleResevereTokenRequest_Implementation(const FRequesterGeneralTokenData& RequesterTokenData, const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData) override;

	virtual FVector HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint) override;


//METHODS (not inherited from abstract) 
	//GETTERS & SETTERS
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsForceFieldActive();
	
	UFUNCTION(BlueprintCallable, Blueprintable)
	void SetWidgetValues();
	
	UFUNCTION(BlueprintCallable)
	void SetForceFieldID(int32 InForceFieldID);

	UFUNCTION(BlueprintCallable)
	void SetIsForceFieldActive(bool InIsForceFieldActive);

	UFUNCTION(BlueprintCallable)
	int32 GetForceFieldTag();


	//DELEGATE BROADCAST
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	UFUNCTION()
	void OnObjectiveTimerCompleteStopSoundTimer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnAndSaveIdOfForceField();

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayNoise();
	
	UFUNCTION(BlueprintCallable)
	void OnGameOverClearTimer();

	UFUNCTION(BlueprintCallable)
	void OnLeverPullRestoreHealthOfForceField();
};
