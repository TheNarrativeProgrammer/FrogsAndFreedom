// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "HealthComponent.h"
#include "../../Interfaces/EnemyDamageable.h"
#include "../../Core/GameStateEventManagerBase.h"
#include "Interfaces/Token/AttackTokenGiverable.h"
#include "Components/TokenComponents/AttackTokenGiverComponent.h"
#include "Components/BindComponents/DelegateBindComponentForceField.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Interfaces/RocketHealable.h"
#include "ForceFieldBase.generated.h"

UCLASS()
class UNREALPROJECTBASE_API AForceFieldBase : public AActor,
public IEnemyDamageable,
public IDelegateBindBroadcastable,
public IGameplayTagAssetInterface,
public IGenericTeamAgentInterface,
public IRocketHealable,
public IAttackTokenGiverable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForceFieldBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraComponent* NiagaraComponentForceField;

	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackTokenGiverComponent> AttackTokenGiverableComponent;

	//Function - inherits from IGenericTeamAgentInterface. IdNumber is public and is used to change the TeamId variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 IdNumber;

	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }
	

	UFUNCTION()
	void RestoreHealth();

	// UFUNCTION()
	// void PopulateEnemyAttackLocationsArray(USceneComponent* EnemyAttackLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterStartBroadcastActorInitializationDelegates();
	//
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// TArray<FVector> GetEnemyAttackLocations();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlayTagContainer")
	FGameplayTagContainer GameplayTagContainer;

	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	//PROPERTIES - team ID for GenericTeamAgentInterface
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGenericTeamId TeamId;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();
	
	UFUNCTION()
	void Death();

	// UFUNCTION()
	// void OnHealthChangeSendHealth(float CurrentHealth, float OldHealth);

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentForceField> DelegateBindComponentForceField;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraComponent* NiagaraComponentCracks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool _mIsForceFieldActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radius Niagara")
	float RadiusNiagara = 600;

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

	//PROPERIES - enemy attack location
	UPROPERTY()
	TArray<USceneComponent*> _maEnemyAttackLocations;


	

public:	


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Test")
	int32 _mForceFieldID;

	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();
	
	

	UFUNCTION()
	void OnHealthChangeSendIdAndHealth(uint32 ActorId, float NewHealth, float OldHealth);
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterEndDestroyThis();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeactivateComponents();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ActivateComponents();
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGeneratorInteractRestoreHealthAndActivateIfIdMatches(int32 ActorID);
	
	UFUNCTION(BlueprintCallable)
	void SetIsForceFieldActive(bool IsForceFieldActive);

	UFUNCTION(BlueprintCallable)
	bool GetIsForceFieldActive();
	

	UFUNCTION(BlueprintCallable)
	void SetIsActiveOnEncounterBegin(bool InIsActiveOnEncounterBegin);


	//IAttackTokenGiverable Function implementation
	virtual void HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint) override;

	virtual bool HandleResevereTokenRequest_Implementation(const FRequesterGeneralTokenData& RequesterTokenData, const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData) override;

	virtual FVector HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint) override;

};
