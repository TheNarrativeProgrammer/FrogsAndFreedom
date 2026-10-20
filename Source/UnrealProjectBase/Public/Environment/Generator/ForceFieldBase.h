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
	
//PROPERTIES
	//components
	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackTokenGiverComponent> AttackTokenGiverableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentForceField> DelegateBindComponentForceField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraComponent> NiagaraComponentCracks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentRoot;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	
	//IdNumber is public and is used to change the TeamId variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 IdNumber;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlayTagContainer")
	FGameplayTagContainer GameplayTagContainer;

protected:
	//force field
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool _mIsForceFieldActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radius Niagara")
	float RadiusNiagara = 600;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Test")
	int32 _mForceFieldID;

	// enemy attack location
	UPROPERTY()
	TArray<USceneComponent*> _maEnemyAttackLocations;
	
	//team ID for GenericTeamAgentInterface
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGenericTeamId TeamId;
	

//FUNCTIONS
public:
	//getters and setters
	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }

	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();

	//health
	UFUNCTION()
	void RestoreHealth();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	UFUNCTION()
	void Death();

	
public:
	//delegate broadcast
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterStartBroadcastActorInitializationDelegates();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	
	UFUNCTION()
	void OnHealthChangeSendIdAndHealth(float NewHealth, float OldHealth);
	
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
