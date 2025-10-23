// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FrogAIable.generated.h"




UENUM(BlueprintType)
enum class EMovementSpeedType: uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging	UMETA(DisplayName = "Jogging"),
	Running UMETA(DisplayName = "Running"),
};

UENUM(BlueprintType)
enum EAIStates : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Passive UMETA(DisplayName = "Passive"),
	Investigate UMETA(DisplayName = "Investigate"),
	Charging UMETA(DisplayName = "Charging"),
	Attacking UMETA(DisplayName = "Attacking"),
	FindCover UMETA(DisplayName = "FindCover"),
	FindAdvantage UMETA(DisplayName = "FindAdvantage"),
	Frozen UMETA(DisplayName = "Frozen"),
	Dead UMETA(DisplayName = "Dead"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UFrogAIable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IFrogAIable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JumpNormalUpward();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JumpSpecialToDestination(FVector DestinationLocation, float ZaxisOffset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JumpSpecialToDestinationForwardBoost(FVector DestinationLocation, float ZaxisOffset, float XaxisOffset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ScaleJumpZVelocity(int32 ScaleMultiplier);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetJumpZVelocityToDefault();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLandedOffNavFireLandedDelegate();
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsCharging() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetHasFinishedCharging() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsFlailing() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetBlackBoardKeys();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindToOnAttackBehaviourFinishedDelegate(UObject* ListenerObject, const FName& FunctionNameToBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnbindFromAttackBehaviourFinishedDelegate(UObject* ListenerObject, const FName& FunctionNameToUnBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindToOnChargeBehaviourFinishedDelegate(UObject* ListenerObject, const FName& FunctionNameToBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnbindFromChargeBehaviourFinishedDelegate(UObject* ListenerObject, const FName& FunctionNameToUnBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindToOnInvestigateBehaviourFinishedDelegate(UObject* ListenerObject, const FName& FunctionNameToBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnbindFromInvestigatekBehaviourFinishedDelegate(UObject* ListenerObject, const FName& FunctionNameToUnBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindToOnReachJumpApexDelegate(UObject* ListenerObject, const FName& FunctionNameToBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnbindFromReachJumpApexDelegate(UObject* ListenerObject, const FName& FunctionNameToUnBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindToLandedDelegate(UObject* ListenerObject, const FName& FunctionNameToBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnbindFromLandedDelegate(UObject* ListenerObject, const FName& FunctionNameToUnBind);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsAttacking() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsGrounded(bool IsGrounded);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTargetActor(AActor* InTargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetNofityJumpApex(bool InNotifyJumpApex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBehaviourChangeUpdateVisualFeedback();

	

	

	
	

	
};
