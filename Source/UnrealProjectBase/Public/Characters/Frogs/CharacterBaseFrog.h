// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/Poolable.h"
#include "../../Interfaces/PlayerDamageable.h"
#include "Core/GameStateEventManagerBase.h"
#include "Interfaces/Token/AttackTokenRequesterable.h"
#include "Launchable.h"
#include "Components/TokenComponents/AttackTokenRequesterComponent.h"

#include "Components/BindComponents/DelegateBindingComponentFrog.h"
#include "Interfaces/FrogAIable.h"
#include "Components/MaterialBillboardComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "CharacterBaseFrog.generated.h"

/**
 * 
 */

// UENUM()
// enum eAttackState : int8
// {
// 	Roaming,
// 	Charging,
// 	Attacking,
// };

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackBehaviourFinishedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeBehaviourFinishedSignature, bool, IsChargeSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvestigateBehaviourFinishedSignature);


UCLASS()
class UNREALPROJECTBASE_API ACharacterBaseFrog
	: public ACharacterBase,
public IPoolable,
public IPlayerDamageable,
public IFrogAIable,
public IAttackTokenRequesterable
{
	GENERATED_BODY()
public:
	ACharacterBaseFrog();

	void Landed(const FHitResult& Hit) override;

	// UPROPERTY()
	// AGameStateEventManagerBase* GameStateEventManager;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	// void OnPoolActorActivated();
	

	//IPoolable functions
	virtual void OnPoolActorActivated_Implementation() override;

	virtual void OnPoolActorDeactivated_Implementation() override;

	UPROPERTY(BlueprintReadWrite)
	bool IsAttacking = false;


	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindingComponentFrog> DelegateBindingComponentFrog;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UMaterialBillboardComponent* MaterialBillboardComponentAggression;

	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackTokenRequesterComponent> AttackTokenRequesterComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shielded State")
	bool _mIsShielded  = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shielded State")
	bool _mIsFlailing = false;

	//PlayerDamageable interface functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsShielded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsShielded(bool InIsShielded);
	
	

	
	

	UPROPERTY()
	FTimerHandle CallBackDeactivateRotation;

	UFUNCTION()
	void DeactivateRotationComponent();

	//TODO remove this code for IPatrolRoutable once it's comfirmed nothing is broken. This interface shouldn't be on the tank.
	//Interface is implemented on flusher and pinner.
	
	//PatrolRouteable interface functions
	// virtual APatrolRouteBase* GetPatrolRoute_Implementation() override;;

	// virtual void SetPatrolRoute_Implementation(APatrolRouteBase* InPatrolRoute) override;

	// virtual void IncrumentPatrolRouteIndex_Implementation() override;

	// virtual int32 GetPatrolIndex_Implementation() override
	// {
	// 	return _mPatrolIndex;
	// };

	// UPROPERTY(BlueprintReadWrite)
	// FVector _mPatrolPointQueryLocation;

	// void SetPatrolPointQueryLocation_Implementation(FVector InPatrolPointQueryLocation) override;
	//
	// FVector GetPatrolPointQueryLocation_Implementation() override;

	//AttackTokenRequesterable functions
	virtual bool RequestAttackToken_Implementation(AActor* AttackTarget) override;

	virtual void ReturnAttackTokens_Implementation(AActor* AttackTarget) override;

	virtual void ReturnAllAttackTokensForAllTargets_Implementation() override;

	virtual bool HasAttackToken_Implementation(AActor* AttackTarget) override;

	virtual FVector RequestAttackLocation_Implementation(AActor* AttackTarget) override;

	virtual void ClearTokenMap_Implementation() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Route Variables")
	int32 _mPatrolIndex;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Route Variables", meta = (ClampMin = "-1", ClampMax = "1", UIMin = "-1", UIMax = "1"))
	int32 _mDirection = 1;

	

	

	

	//Feedback for player
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateColorBasedOnAttackState();

	UFUNCTION(BlueprintCallable)
	void OnBehaviourTestStateToToggleVisualFeedback();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ToggleAggressionBilloard(bool IsBillboardHidden);

	
	virtual void BeginPlay() override;

	//Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mTimeBetweenChargeAndAttack = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mAttackRangeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mAttackRangeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mMinimumDotProductForAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> mPlayerPawn;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _mIsEngaged = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool mIsGrounded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool mIsDead;

	//Properties - walk speeds
	UPROPERTY(VisibleAnywhere)
	float _mOriginalWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed")
	float _mSpeedIdle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed")
	float _mSpeedWalking = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed")
	float _mSpeedJogging = 2600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Speed")
	float _mSpeedRunning = 4000.0f;

	
	

	


	//Properties - AI State
	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameIsGrounded = "IsGrounded";

	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameIsCharged = "IsCharged";

	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameIsCharging = "IsCharging";

	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameState = "State";

	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameIsEngaged = "IsEngaged";

	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameIsAttacking = "IsAttacking";

	UPROPERTY(BlueprintReadOnly, Category="AI")
	FName BBKeyNameIsDead = "IsDead";

	//property - FindRotationAimedAtFuturePlayerLocation
	UPROPERTY(BlueprintReadOnly, Category="AI")
	float _mSpeedToProjectileOrAttack = 1000;

	UPROPERTY(BlueprintReadOnly, Category="AI")
	float _mPredictionReductionMultiplier = 0.5;

	//Functions - AI State
	UFUNCTION(BlueprintCallable, Category="AI")
	bool TestAIState(EAIStates InAIStateToTestFor);

	UFUNCTION(BlueprintCallable, Category="AI")
	void ChangeBlackboardKeyBool(FName KeyToChange, bool InKeyValue);

	UFUNCTION(BlueprintCallable, Category="AI")
	void ChangeBlackboardKeyEAIState(EAIStates NewState);

	//Functions - component init

	UFUNCTION()
	void InitializeComponents();

	UFUNCTION()
	void InitializeReferences();

	//Functions Poolable interface
	UFUNCTION()
	void OnDeathReturnToPool();

	UFUNCTION()
	virtual void Death();

	UFUNCTION()
	void BindHealthComponent();

	UFUNCTION(BlueprintCallable)
	void JumpToDestination(FVector DestinationLocation, float ZAxisOffset, float gravityOverRide);


	//FrogAIable Interface functions

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLandedOffNavFireLandedDelegate_Implementation() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float SetMovementSpeed(EMovementSpeedType MovementSpeedType);


	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IdealBehaviour();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PassiveBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InvestigateBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChargingBehaviour();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackBehaviour(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MidAttackChargePreDamageBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FindCoverBehaviour(AActor* CoverTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FindAdvantageBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FrozenBehaviour();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeadBehaviour();

	virtual void OnBehaviourChangeUpdateVisualFeedback_Implementation() override;

	virtual bool GetIsCharging_Implementation() const override
	{
		return _mIsCharging;
	};

	virtual bool GetIsFlailing_Implementation() const override
	{
		return _mIsFlailing;
	};

	virtual bool GetHasFinishedCharging_Implementation() const override
	{
		return _mHasFinishedCharging;
	};

	virtual void ResetBlackBoardKeys_Implementation() override;

	virtual bool GetIsAttacking_Implementation() const override
	{
		return IsAttacking;
	};

	virtual void JumpNormalUpward_Implementation() override;

	virtual void JumpSpecialToDestination_Implementation(FVector DestinationLocation, float ZaxisOffset) override;

	virtual void JumpSpecialToDestinationForwardBoost_Implementation(FVector DestinationLocation, float ZaxisOffset, float XaxisOffset) override;

	virtual void ScaleJumpZVelocity_Implementation(int32 ScaleMultiplier) override;

	virtual void ResetJumpZVelocityToDefault_Implementation() override;

	UPROPERTY()
	int32 _mDefaultJumpZVelocity;

	virtual void SetTargetActor_Implementation(AActor* InTargetActor) override;

	virtual void SetIsGrounded_Implementation(bool IsGrounded) override;

	virtual void SetNofityJumpApex_Implementation(bool InNotifyJumpApex) override;

	virtual void BindToOnAttackBehaviourFinishedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToBind) override;

	virtual void UnbindFromAttackBehaviourFinishedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToUnBind) override;

	virtual void BindToOnChargeBehaviourFinishedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToBind) override;

	virtual void UnbindFromChargeBehaviourFinishedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToUnBind) override;

	virtual void BindToOnInvestigateBehaviourFinishedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToBind) override;

	virtual void UnbindFromInvestigatekBehaviourFinishedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToUnBind) override;

	virtual void BindToOnReachJumpApexDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToBind) override;

	virtual void UnbindFromReachJumpApexDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToUnBind) override;

	virtual void BindToLandedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToBind) override;

	virtual void UnbindFromLandedDelegate_Implementation(UObject* ListenerObject, const FName& FunctionNameToUnBind) override;


	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintType, BlueprintAssignable, Category="Behaviour Finished Delegates")
	FOnAttackBehaviourFinishedSignature OnAttackBehaviourFinishedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintType, BlueprintAssignable, Category="Behaviour Finished Delegates")
	FOnChargeBehaviourFinishedSignature OnChargeBehaviourFinishedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintType, BlueprintAssignable, Category="Behaviour Finished Delegates")
	FOnInvestigateBehaviourFinishedSignature OnInvestigateBehaviourFinishedDelegate;

	
	//Properties for tracking bool
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Behaviour Tracking Bools")
	bool _mHasFinishedCharging = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Behaviour Tracking Bools")
	bool _mIsCharging = false;

	
	

	//Function - FindRotationAimedAtFuturePlayerLocation
	UFUNCTION(BlueprintCallable)
	FRotator FindRotationAimedAtFuturePlayerLocation(FVector InAttackLocation);

	UFUNCTION(BlueprintCallable)
	void SetSpeedToProjectileOrAttack(float InSpeedToProjectileOrAttack);
	


	//Deprecitated Functions

	UFUNCTION(BlueprintImplementableEvent)
	void PlayJumpAnimation();

	UFUNCTION(BlueprintCallable)
	float GetDistanceToPlayer();

	UFUNCTION(BlueprintCallable)
	bool IsInAttackRange();

	UFUNCTION(BlueprintCallable)
	float GetDotProductToPlayer();

	UFUNCTION(BlueprintCallable)
	bool IsFacingPlayer();

	UFUNCTION(BlueprintCallable)
	void SetIsEngaged(bool InIsEngaged);

	UFUNCTION(BlueprintCallable)
	void StopAttack();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool InIsAttacking);

public:
	
	
	
};
