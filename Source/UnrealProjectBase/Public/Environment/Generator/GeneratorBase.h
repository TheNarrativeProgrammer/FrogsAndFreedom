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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentGenerator> DelegateBindComponentGenerator;
	
	UFUNCTION()
	void CheckForceFieldIDMatchesThisOwner(int32 InForceField);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterStartBroadcastActorInitializationDelegates();

	UFUNCTION(BlueprintCallable)
	void SetSoundTimer();
	
	//Functions - inherited from IGameplayerTagAssetInterface
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	//Function - inherits from IGenericTeamAgentInterface. IdNumber is public and is used to change the TeamId variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 IdNumber;

	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }

	//PROPERTIES - team ID for GenericTeamAgentInterface
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGenericTeamId TeamId;

	
	
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponentJumpOffLocation;

	

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnHealthDepetedForceFieldSignature OnHealthDepetedForceFieldDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayTagContainer")
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY()
	TObjectPtr<UPawnNoiseEmitterComponent> NoiseEmitterComponent;

	

	UFUNCTION(BlueprintCallable)
	bool GetIsForceFieldActive();


	//FUNCTIONS - enemy attack locations
	//  UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//  TArray<FVector> GetEnemyAttackLocations();
	//
	// //FUNCTIONS - enemy attack locations
	//  UFUNCTION()
	//  void PopulateEnemyAttackLocationsArray(USceneComponent* EnemyAttackLocation);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	UFUNCTION()
	void OnObjectiveTimerCompleteStopSoundTimer();

	//FUNCTIONS - Interactable interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseInteractVisual();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractMessageOn(FRotator PlayerRotator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractMessageOff();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnAndSaveIdOfForceField();

	

	//PROPERTIES - components
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackTokenGiverComponent> AttackTokenGiverableComponent;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnmenyAttackLocationTop;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnmenyAttackLocationBottom;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnmenyAttackLocationLeft;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnmenyAttackLocationRight;

	//PROPERTIES - Force field
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Encounter Options")
	bool _mIsShieldActiveOnEncounterStart = false;

	//PROPERTIES - Force field
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ForceFieldID")
	int32 _mForceFieldID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Field Class")
	TSubclassOf<AForceFieldBase> ForceFieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Force Field Class")
	bool _mIsForceFieldActive;

	//PROPERIES - enemy attack location
	// UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "AttackLocations")
	// TArray<USceneComponent*> _maEnemyAttackLocations;

	

	//Functions - GenericTeamAgentInterface
	//virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	

	//FUNCTIONS - register delegates for force field health
	// UFUNCTION(BlueprintCallable)
	// void RegisterListenersForHealthOfForceFields();

	// UFUNCTION()
	// void BroadcastHealthDepletedOfForceField();

	
	

	UFUNCTION(BlueprintCallable)
	void SetForceFieldID(int32 InForceFieldID);

	UFUNCTION(BlueprintCallable)
	void SetIsForceFieldActive(bool InIsForceFieldActive);

	
	

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FTimerHandle GeneratorSoundTimerHandle;

	UPROPERTY()
	float TimeBetweenNoises = 5.0f;
	

	

	UFUNCTION(BlueprintCallable)
	void PlayNoise();

	UFUNCTION(BlueprintCallable)
	int32 GetForceFieldTag();

	//IAttackTokenGiverable Function implementation
	virtual void HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint) override;

	virtual bool HandleResevereTokenRequest_Implementation(const FRequesterGeneralTokenData& RequesterTokenData, const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData) override;

	virtual FVector HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint) override;

	UFUNCTION(BlueprintCallable)
	void OnGameOverClearTimer();

	UFUNCTION(BlueprintCallable)
	void OnLeverPullRestoreHealthOfForceField();
	
};
