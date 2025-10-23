// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Interfaces/AIBehaviourStateable.h"
#include "Perception/AIPerceptionTypes.h"

#include "Interfaces/NavPointFinderable.h"
#include "AIControllerMobileFrogBase.generated.h"

UENUM(BlueprintType, Blueprintable)
enum EAISense : uint8
{
	None UMETA(DisplayName = "None"),
	Sight UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Damage UMETA(DisplayName = "Damage"),
};
/**
 * 
 */


class ACharacterBase;

UCLASS()
class UNREALPROJECTBASE_API AAIControllerMobileFrogBase : public ADetourCrowdAIController,
public INavPointFinderable,
public IAIBehaviourStateable
{
	GENERATED_BODY()
public:
	AAIControllerMobileFrogBase();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	//PROPERTY - used with IGenericTeamAgentInterface 
	UPROPERTY()
	FGenericTeamId TeamId;

	//Functions - inherited from IGenericTeamAgentInterface which is interface in AAIController
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// UPROPERTY(BlueprintReadOnly, Category="Perception Stimulus")
	// FAIStimulus LastStimulus;
	
public:
	//Functions - inherited from IGenericTeamAgentInterface which is interface in AAIController
	virtual FGenericTeamId GetGenericTeamId()const override { return TeamId; }

protected:

	UFUNCTION()
	bool IsUpdatedSenseOfActorOfAGivenType(AActor* Actor, EAISense InSenseToCheck, FAIStimulus& Stimulus);
	
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleSenseSight(AActor* SensedActor);

	UFUNCTION()
	void HandleSenseHearing(AActor* SensedActor);

	UFUNCTION()
	void HandleSenseDamage(AActor* SensedActor);

	UFUNCTION()
	EAIStates GetAICurrentState();

	UFUNCTION()
	void SetBBkeyTargetAndState(AActor* AttackTarget, EAIStates NewState);

	UFUNCTION(BlueprintCallable)
	void GetLastStimulus(AActor* TargetActor, bool& bSensedSuccessfully, FAIStimulus& OutStimulus,  FName& ActorTag, EAISense SenseToQuery);

	UFUNCTION()
	void TargetPerceptionForgotten(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnPerceptionChangeSwitchState(AActor* TargetActor, EAIStates NewState);



public:
	virtual EAIStates GetState_Implementation() const override;

	virtual void OnActorReturnToPoolUnbindFromBehaviourDelegates_Implementation() override;
	
	//PROPERTY - used with IGenericTeamAgentInterface 
	UPROPERTY(BlueprintReadWrite)
	class ACharacterBase* CharacterBaseOwner;

	virtual FVector FindReachablePointWhenOffNav_Implementation() override;

	virtual void ResetStateToPassive_Implementation() override;
	
protected:
	FAISenseID GetSenseIDofSenseAsEnum(EAISense SenseAsEnum);
	

	

	
	
};
