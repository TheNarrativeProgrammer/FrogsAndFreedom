// Copyright Peasant Pals 2025


#include "Characters/Frogs/Enemies/AI/Controllers/AIControllerMobileFrogBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/EnemyDamageable.h"
#include "Interfaces/PlayerCharacterable.h"
#include "Interfaces/Token/AttackTokenRequesterable.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"


AAIControllerMobileFrogBase::AAIControllerMobileFrogBase()
{
	
}

void AAIControllerMobileFrogBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAIControllerMobileFrogBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(InPawn))
	{
		CharacterBaseOwner = CharacterBase;
		TeamId = FGenericTeamId(CharacterBaseOwner->GetGenericTeamId());
	}
	if (UAIPerceptionComponent* AIPerceptionComponent = GetComponentByClass<UAIPerceptionComponent>())
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic
			(this, &AAIControllerMobileFrogBase::TargetPerceptionUpdated);
		AIPerceptionComponent->OnTargetPerceptionForgotten.AddUniqueDynamic
			(this, &AAIControllerMobileFrogBase::TargetPerceptionForgotten);
	}
}

ETeamAttitude::Type AAIControllerMobileFrogBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (Other.Implements<UGenericTeamAgentInterface>())
	{
		FGenericTeamId OtherActorsTeamId = NULL;
		const IGenericTeamAgentInterface* GenericTeamAgentInterface = Cast<IGenericTeamAgentInterface>(&Other);
		if (GenericTeamAgentInterface)
		{
			OtherActorsTeamId = GenericTeamAgentInterface->GetGenericTeamId();
			if (OtherActorsTeamId != TeamId)
			{
				return ETeamAttitude::Hostile;
			}
			else if (OtherActorsTeamId == TeamId)
			{
				return ETeamAttitude::Friendly;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

bool AAIControllerMobileFrogBase::IsUpdatedSenseOfActorOfAGivenType(
	AActor* Actor,EAISense InSenseToCheck,FAIStimulus& Stimulus)
{
	if (UAIPerceptionComponent* AIPerceptionComponent = GetAIPerceptionComponent())
	{
		FActorPerceptionBlueprintInfo Info;
		bool IsInfoFound = AIPerceptionComponent->GetActorsPerception(Actor, Info);
		if (IsInfoFound == true)
		{
			if (Info.bIsHostile)
			{
				if (Stimulus.Type == GetSenseIDofSenseAsEnum(InSenseToCheck))
				{
					return true;
				}
			}
		}
	}
	return false;
}


void AAIControllerMobileFrogBase::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		bool IsSightSuccessfullySensed = IsUpdatedSenseOfActorOfAGivenType(Actor, EAISense::Sight, Stimulus);
		if (IsSightSuccessfullySensed == true)
		{
			HandleSenseSight(Actor);
			return;
		}
		bool IsHearingSuccessfullySensed = IsUpdatedSenseOfActorOfAGivenType(Actor, EAISense::Hearing, Stimulus);
		if (IsHearingSuccessfullySensed == true)
		{
			HandleSenseHearing(Actor);
			return;
		}
		bool IsDamageSuccessfullySensed = IsUpdatedSenseOfActorOfAGivenType(Actor, EAISense::Damage, Stimulus);
		if (IsDamageSuccessfullySensed == true)
		{
			HandleSenseDamage(Actor);
		}
	}
}


void AAIControllerMobileFrogBase::HandleSenseSight(AActor* SensedActor)
{
	if (SensedActor->Implements<UPlayerCharacterable>())
	{
		EAIStates CurrentState = GetAICurrentState();
		if (CurrentState != EAIStates::Attacking && CurrentState != EAIStates::Dead)
		{
			SetBBkeyTargetAndState(SensedActor, EAIStates::Attacking);
		}
	}
}
EAIStates AAIControllerMobileFrogBase::GetAICurrentState()
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		uint8 EnumValue = BlackboardComponent->GetValueAsEnum(FName("State"));
		return static_cast<EAIStates>(EnumValue);
	}
	return EAIStates::Passive;
}
void AAIControllerMobileFrogBase::SetBBkeyTargetAndState(AActor* AttackTarget, EAIStates NewState)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsObject(FName("TargetObject"), AttackTarget);
		BlackboardComponent->SetValueAsEnum(FName("State"), NewState);
	}
}

void AAIControllerMobileFrogBase::HandleSenseHearing(AActor* SensedActor)
{
	EAIStates CurrentState = GetAICurrentState();
	if (CurrentState != EAIStates::Investigate && CurrentState != EAIStates::Dead)
	{
		SetBBkeyTargetAndState(SensedActor, EAIStates::Investigate);
	}
}

void AAIControllerMobileFrogBase::HandleSenseDamage(AActor* SensedActor)
{
	EAIStates CurrentState = GetAICurrentState();
	if (CurrentState != EAIStates::Attacking && CurrentState != EAIStates::Dead)
	{
		SetBBkeyTargetAndState(SensedActor, EAIStates::Attacking);
	}
}



void AAIControllerMobileFrogBase::GetLastStimulus(AActor* TargetActor, bool& bSensedSuccessfully, FAIStimulus& OutStimulus, FName& ActorTag, EAISense SenseToQuery)
{
	bSensedSuccessfully = false;
	ActorTag = NAME_None;
	
	if (UAIPerceptionComponent* AIPerceptionComponent = GetComponentByClass<UAIPerceptionComponent>())
	{
		if (!TargetActor || !AIPerceptionComponent)
		{
			return;
		}
		FActorPerceptionBlueprintInfo Info;
		AIPerceptionComponent->GetActorsPerception(TargetActor, Info);

		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type == GetSenseIDofSenseAsEnum(SenseToQuery))
			{
				OutStimulus = Stimulus;
				bSensedSuccessfully = Stimulus.WasSuccessfullySensed();

				if (TargetActor->Tags.Num() > 0)
				{
					ActorTag = TargetActor->Tags[0];
				}
				return;
			}
		}
	}
	
	
	
}

void AAIControllerMobileFrogBase::TargetPerceptionForgotten(AActor* TargetActor)
{
	bool IsSensedSuccessfully = false;
	FAIStimulus OutStimulus;
	FName ActorTag = NAME_None;
	EAISense SenseToQuery = EAISense::Sight;
	AActor* ActorPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (ActorPlayer)
	{
		GetLastStimulus(ActorPlayer, IsSensedSuccessfully, OutStimulus, ActorTag, SenseToQuery);
	}
	
	if (IsSensedSuccessfully == false)
	{
		SetBBkeyTargetAndState(nullptr, EAIStates::Passive);
		if (CharacterBaseOwner)
		{
			IAttackTokenRequesterable::Execute_ReturnAllAttackTokensForAllTargets(CharacterBaseOwner);
		}
	}
}
void AAIControllerMobileFrogBase::OnPerceptionChangeSwitchState(AActor* TargetActor, EAIStates NewState)
{
	if (TargetActor->Implements<UEnemyDamageable>())
	{
			SetBBkeyTargetAndState(TargetActor, NewState);
	}
}



EAIStates AAIControllerMobileFrogBase::GetState_Implementation() const
{
	return IAIBehaviourStateable::GetState_Implementation();
}

void AAIControllerMobileFrogBase::OnActorReturnToPoolUnbindFromBehaviourDelegates_Implementation()
{
}



FVector AAIControllerMobileFrogBase::FindReachablePointWhenOffNav_Implementation()
{
	APawn* OwningPawn = GetPawn();
	if (!OwningPawn) return FVector::ZeroVector;
	
	if (UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation FoundLocation;
		float radiusOfSearch = 1000;
		
		if (OwningPawn)
		{
			bool IsFound = NavSystem->GetRandomPointInNavigableRadius(OwningPawn->GetActorLocation(), radiusOfSearch, FoundLocation);

			if (IsFound == true)
			{
				return FoundLocation.Location;
			}
		}
		
	}

	FVector AltLocation = OwningPawn->GetActorLocation() + OwningPawn->GetActorForwardVector() * 100.f;
	return AltLocation;
}

void AAIControllerMobileFrogBase::ResetStateToPassive_Implementation()
{
	INavPointFinderable::ResetStateToPassive_Implementation();
}

FAISenseID AAIControllerMobileFrogBase::GetSenseIDofSenseAsEnum(EAISense SenseAsEnum)
{
	switch (SenseAsEnum)
	{
	case EAISense::Sight:
		{
			return UAISense::GetSenseID<UAISense_Sight>();
		}
	case EAISense::Hearing:
		{
			return UAISense::GetSenseID<UAISense_Hearing>();
		}
	case EAISense::Damage:
		{
			return UAISense::GetSenseID<UAISense_Damage>();
		}
	default:
		{
			return UAISense::GetSenseID<UAISense_Sight>();
		}
	}
}







