// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Frogs/CharacterBaseFrog.h"


#include "GameFramework/CharacterMovementComponent.h"
#include "Subsystems/ObjectPoolingSubsystem.h"
#include "Environment/PatrolRoutes/PatrolRouteBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACharacterBaseFrog::ACharacterBaseFrog()
{
	PrimaryActorTick.bCanEverTick = true;
	//Create components
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MaterialBillboardComponentAggression = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboardAggression"));
	DelegateBindingComponentFrog = CreateDefaultSubobject<UDelegateBindingComponentFrog>(TEXT("DelegateBindingComponentFrog"));
	AttackTokenRequesterComponent = CreateDefaultSubobject<UAttackTokenRequesterComponent>(TEXT("AttackTokenRequesterComponent"));

	RotatingMovementComponent  = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 900.0f, 0.0f);
	
	//Attach components to root
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	MaterialBillboardComponentAggression->SetupAttachment(StaticMeshComponent);

	//bind and variables for Jump functionality
	GetCharacterMovement()->bNotifyApex = true;
	
}

void ACharacterBaseFrog::Landed(const FHitResult& Hit)
{
	
	//Super::Landed(Hit);
	_mIsFlailing = false;
	Execute_SetIsGrounded(this, true);
	//OnLanded(Hit);
	DeactivateRotationComponent();
	
	AActor* ActorHit = Hit.GetActor();
	if (ACharacterBaseFrog* CharacterBaseFrog = Cast<ACharacterBaseFrog>(ActorHit))
	{
		//UE_LOG(LogTemp, Display, TEXT("Landed on a frog"));
		return;
	}
	LandedDelegate.Broadcast(Hit);
	
}

void ACharacterBaseFrog::OnPoolActorActivated_Implementation()
{
	mIsDead = false;
	mIsGrounded = false;
	_mIsFlailing = false;
	_mIsEngaged = false;
	TargetActor = nullptr;
	IPoolable::OnPoolActorActivated_Implementation();
}

void ACharacterBaseFrog::OnPoolActorDeactivated_Implementation()
{
	IPoolable::OnPoolActorDeactivated_Implementation();
	// Clear stale behaviour-delegate bindings so a reused (pooled) enemy never broadcasts to a previous life's BTT listeners.
	OnAttackBehaviourFinishedDelegate.Clear();
	OnChargeBehaviourFinishedDelegate.Clear();
	OnInvestigateBehaviourFinishedDelegate.Clear();
	OnReachedJumpApex.Clear();
	LandedDelegate.Clear();
	if (!AttackTokenRequesterComponent) return;
	AttackTokenRequesterComponent->ReturnAllAttackTokensForAllTargets();
	AttackTokenRequesterComponent->ClearTokenMap();
	DeactivateRotationComponent();
}


void ACharacterBaseFrog::SetIsShielded_Implementation(bool InIsShielded)
{
	_mIsShielded = InIsShielded;
}


bool ACharacterBaseFrog::GetIsShielded_Implementation()
{
	return _mIsShielded;
}

// void ACharacterBaseFrog::IncrumentPatrolRouteIndex_Implementation()
// {
// 	IPatrolRouteable::IncrumentPatrolRouteIndex_Implementation();
// 	PatrolRouteBase->IncrumentPatrolRoute(_mPatrolIndex, _mDirection);
// }

// void ACharacterBaseFrog::SetPatrolRoute_Implementation(APatrolRouteBase* InPatrolRoute)
// {
// 	IPatrolRouteable::SetPatrolRoute_Implementation(InPatrolRoute);
// 	PatrolRouteBase = InPatrolRoute;
// }

// APatrolRouteBase* ACharacterBaseFrog::GetPatrolRoute_Implementation()
// {
// 	IPatrolRouteable::GetPatrolRoute_Implementation();
// 	return PatrolRouteBase;
// }

// void ACharacterBaseFrog::OnPoolActorActivated_Implementation()
// {
// 	IPoolable::OnPoolActorActivated_Implementation();
// 	GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.Broadcast();
// }










void ACharacterBaseFrog::DeactivateRotationComponent()
{
	RotatingMovementComponent->Deactivate();
}


// void ACharacterBaseFrog::SetPatrolPointQueryLocation_Implementation(FVector InPatrolPointQueryLocation)
// {
// 	IPatrolRouteable::SetPatrolPointQueryLocation_Implementation(InPatrolPointQueryLocation);
// 	_mPatrolPointQueryLocation = InPatrolPointQueryLocation;
// 	
// }
//
// FVector ACharacterBaseFrog::GetPatrolPointQueryLocation_Implementation()
// {
// 	IPatrolRouteable::GetPatrolPointQueryLocation_Implementation();
// 	return _mPatrolPointQueryLocation;
// }

bool ACharacterBaseFrog::RequestAttackToken_Implementation(AActor* AttackTarget)
{
	if (!AttackTarget || !AttackTokenRequesterComponent) return false;
	
	return AttackTokenRequesterComponent->RequestAttackToken(AttackTarget);
}

void ACharacterBaseFrog::ReturnAttackTokens_Implementation(AActor* AttackTarget)
{
	if (!AttackTarget || !AttackTokenRequesterComponent) return;
	
	AttackTokenRequesterComponent->ReturnAttackTokens(AttackTarget);
}

void ACharacterBaseFrog::ReturnAllAttackTokensForAllTargets_Implementation()
{
	if (!AttackTokenRequesterComponent) return;
	AttackTokenRequesterComponent->ReturnAllAttackTokensForAllTargets();
}

bool ACharacterBaseFrog::HasAttackToken_Implementation(AActor* AttackTarget)
{
	if (!AttackTarget || !AttackTokenRequesterComponent)return false;
	return AttackTokenRequesterComponent->HasAttackTokens(AttackTarget);
}

FVector ACharacterBaseFrog::RequestAttackLocation_Implementation(AActor* AttackTarget)
{
	if (!AttackTarget || !AttackTokenRequesterComponent) return FVector::ZeroVector;
	return AttackTokenRequesterComponent->RequestAttackLocation(AttackTarget);
}

void ACharacterBaseFrog::ClearTokenMap_Implementation()
{
	if (!AttackTokenRequesterComponent) return;
	AttackTokenRequesterComponent->ClearTokenMap();
}


void ACharacterBaseFrog::OnBehaviourTestStateToToggleVisualFeedback()
{
	//Aggravation billboard toggle
	bool IsInvestigating = TestAIState(EAIStates::Investigate);
	if (IsInvestigating == true)
	{
		//MaterialBillboardComponent->SetHiddenInGame(false);
		ToggleAggressionBilloard(false);
	}
	else
	{
		//MaterialBillboardComponent->SetVisibility(false);
		ToggleAggressionBilloard(true);
	}
}

void ACharacterBaseFrog::ToggleAggressionBilloard_Implementation(bool IsBillboardHidden)
{
}

void ACharacterBaseFrog::BeginPlay()
{
	Super::BeginPlay();

	DeactivateRotationComponent();
	BindHealthComponent();
	
	InitializeComponents();
	InitializeReferences();

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		_mDefaultJumpZVelocity = CharacterMovementComponent->JumpZVelocity;
	}
	
}

bool ACharacterBaseFrog::TestAIState(EAIStates InAIStateToTestFor)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			EAIStates CurrentState = static_cast<EAIStates>(BlackboardComponent->GetValueAsEnum("State"));
			return CurrentState == InAIStateToTestFor;
		}
	}
	return false;
}

void ACharacterBaseFrog::ChangeBlackboardKeyBool(FName KeyToChange, bool InKeyValue)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsBool(KeyToChange, InKeyValue);
		}
	}
}

void ACharacterBaseFrog::ChangeBlackboardKeyEAIState(EAIStates NewState)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsEnum("State", NewState);
		}
	}
}

void ACharacterBaseFrog::InitializeComponents()
{
	
}

void ACharacterBaseFrog::InitializeReferences()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		mPlayerPawn = PlayerController->GetPawn();
	}
}

void ACharacterBaseFrog::OnDeathReturnToPool()
{
	if (IsValid(this) == false || HasAnyFlags(RF_BeginDestroyed) == true) return;
	
	mIsDead = true;
	if (UObjectPoolingSubsystem* ObjectPoolingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolingSubsystem>())
	{
		ObjectPoolingSubsystem->ReturnToPool(this);
	}
}

void ACharacterBaseFrog::Death()
{
	DelegateBindingComponentFrog->GameStateEventManager->OnCharacterBaseFrogDeathDelegate.Broadcast();
}

void ACharacterBaseFrog::BindHealthComponent()
{
	HealthComponent->OnHealthDepletedDelegate.AddUniqueDynamic(this, &ACharacterBaseFrog::OnDeathReturnToPool);
	HealthComponent->OnHealthDepletedDelegate.AddUniqueDynamic(this, &ACharacterBaseFrog::Death);
}



void ACharacterBaseFrog::InvestigateBehaviour_Implementation()
{
	IFrogAIable::InvestigateBehaviour_Implementation();
}

void ACharacterBaseFrog::AttackBehaviour_Implementation(AActor* AttackTarget)
{
	IFrogAIable::AttackBehaviour_Implementation(AttackTarget);
	
}



void ACharacterBaseFrog::ChargingBehaviour_Implementation()
{
	IFrogAIable::ChargingBehaviour_Implementation();
}

void ACharacterBaseFrog::PassiveBehaviour_Implementation()
{
	IFrogAIable::PassiveBehaviour_Implementation();
}

void ACharacterBaseFrog::IdealBehaviour_Implementation()
{
	IFrogAIable::IdealBehaviour_Implementation();
}

void ACharacterBaseFrog::FindCoverBehaviour_Implementation(AActor* CoverTarget)
{
	IFrogAIable::FindCoverBehaviour_Implementation(CoverTarget);
}


void ACharacterBaseFrog::FindAdvantageBehaviour_Implementation()
{
	IFrogAIable::FindAdvantageBehaviour_Implementation();
}

void ACharacterBaseFrog::FrozenBehaviour_Implementation()
{
	IFrogAIable::FrozenBehaviour_Implementation();
}

void ACharacterBaseFrog::DeadBehaviour_Implementation()
{
	IFrogAIable::DeadBehaviour_Implementation();
}




//FUNCTIONS - FROGAIABLE INTERFACE
float ACharacterBaseFrog::SetMovementSpeed_Implementation(EMovementSpeedType MovementSpeedType)
{
	IFrogAIable::SetMovementSpeed_Implementation(MovementSpeedType);
	float NewWalkSpeed = 0.0f;
	
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		switch (MovementSpeedType)
		{
		case EMovementSpeedType::Idle:
			{
				NewWalkSpeed = _mSpeedIdle;
				break;
			}
		case EMovementSpeedType::Walking:
			{
				NewWalkSpeed = _mSpeedWalking;
				break;
			}
		case EMovementSpeedType::Jogging:
			{
				NewWalkSpeed = _mSpeedJogging;
				break;
			}
		case EMovementSpeedType::Running:
			{
				NewWalkSpeed = _mSpeedRunning;
				break;
			}
		default:
			{
				NewWalkSpeed = 500.0f;
				break;
			}
		}
		CharacterMovementComponent->MaxWalkSpeed = NewWalkSpeed;
	}
	return NewWalkSpeed;
}


void ACharacterBaseFrog::MidAttackChargePreDamageBehaviour_Implementation()
{
	IFrogAIable::MidAttackChargePreDamageBehaviour_Implementation();
}


void ACharacterBaseFrog::OnBehaviourChangeUpdateVisualFeedback_Implementation()
{
	IFrogAIable::OnBehaviourChangeUpdateVisualFeedback_Implementation();
}

void ACharacterBaseFrog::ResetBlackBoardKeys_Implementation()
{
	IFrogAIable::ResetBlackBoardKeys_Implementation();
}


void ACharacterBaseFrog::JumpNormalUpward_Implementation()
{
	Execute_SetIsGrounded(this, false);
	Jump();
}

void ACharacterBaseFrog::JumpSpecialToDestination_Implementation(FVector DestinationLocation, float ZaxisOffset)
{
	JumpToDestination(DestinationLocation, ZaxisOffset, 0.0f);
}
void ACharacterBaseFrog::JumpToDestination(FVector DestinationLocation, float ZAxisOffset, float gravityOverRide)
{
	FVector OutLaunchVelocity;
	DestinationLocation.Z += ZAxisOffset;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, OutLaunchVelocity,
		GetActorLocation(), DestinationLocation, gravityOverRide, 0.5);
	LaunchCharacter(OutLaunchVelocity, true, true);
	Execute_SetIsGrounded(this, false);
	PlayJumpAnimation();
}

void ACharacterBaseFrog::OnLandedOffNavFireLandedDelegate_Implementation_Implementation()
{
	FHitResult HitResult;
	LandedDelegate.Broadcast(HitResult);
}

void ACharacterBaseFrog::JumpSpecialToDestinationForwardBoost_Implementation(FVector DestinationLocation,
                                                                             float ZaxisOffset, float XaxisOffset)
{
	Execute_SetIsGrounded(this, false);
	JumpToDestination(DestinationLocation, ZaxisOffset, 0.0f);

	FTimerHandle TimerHandle;
	FVector LaunchVelocity = GetActorForwardVector() * XaxisOffset;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, LaunchVelocity]{ LaunchCharacter(LaunchVelocity, true, true);}, 0.45f, false);
}


void ACharacterBaseFrog::ScaleJumpZVelocity_Implementation(int32 ScaleMultiplier)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = ScaleMultiplier * _mDefaultJumpZVelocity;
	}
}

void ACharacterBaseFrog::ResetJumpZVelocityToDefault_Implementation()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = _mDefaultJumpZVelocity;
	}
}

void ACharacterBaseFrog::SetTargetActor_Implementation(AActor* InTargetActor)
{
	if (IsValid(InTargetActor) == true)
	{
		TargetActor = InTargetActor;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Target actor null"));
	}
}

void ACharacterBaseFrog::SetIsGrounded_Implementation(bool IsGrounded)
{
	mIsGrounded = IsGrounded;
}

void ACharacterBaseFrog::SetNofityJumpApex_Implementation(bool InNotifyJumpApex)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bNotifyApex = InNotifyJumpApex;
	}
}


void ACharacterBaseFrog::SetIsAttacking(bool InIsAttacking)
{
	IsAttacking = InIsAttacking;
}



void ACharacterBaseFrog::BindToOnAttackBehaviourFinishedDelegate_Implementation(
	UObject* ListenerObject,const FName& FunctionNameToBind)
{
	if (!ListenerObject || FunctionNameToBind.IsNone()) return;
	
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToBind);
	
	if (OnAttackBehaviourFinishedDelegate.Contains(Delelate) == false)
	{
		OnAttackBehaviourFinishedDelegate.AddUnique(Delelate);
	}
}

void ACharacterBaseFrog::UnbindFromAttackBehaviourFinishedDelegate_Implementation(
	UObject* ListenerObject,const FName& FunctionNameToUnBind)
 {
 	if (!ListenerObject || FunctionNameToUnBind.IsNone()) return;
 	
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToUnBind);
	
	if (OnAttackBehaviourFinishedDelegate.Contains(Delelate) == true)
	{
		OnAttackBehaviourFinishedDelegate.Remove(Delelate);
	}
 }

void ACharacterBaseFrog::BindToOnChargeBehaviourFinishedDelegate_Implementation(UObject* ListenerObject,
	const FName& FunctionNameToBind)
{
	if (!ListenerObject || FunctionNameToBind.IsNone())
	{
		return;
	}

	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToBind);
	
	if (OnChargeBehaviourFinishedDelegate.Contains(Delelate) == false)
	{
		OnChargeBehaviourFinishedDelegate.AddUnique(Delelate);
	}
}

void ACharacterBaseFrog::UnbindFromChargeBehaviourFinishedDelegate_Implementation(UObject* ListenerObject,
	const FName& FunctionNameToUnBind)
{
	if (!ListenerObject || FunctionNameToUnBind.IsNone())
	{
		return;
	}
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToUnBind);
	
	if (OnChargeBehaviourFinishedDelegate.Contains(Delelate) == true)
	{
		OnChargeBehaviourFinishedDelegate.Remove(Delelate);
	}
}

void ACharacterBaseFrog::BindToOnInvestigateBehaviourFinishedDelegate_Implementation(UObject* ListenerObject,
	const FName& FunctionNameToBind)
{
	if (!ListenerObject || FunctionNameToBind.IsNone())
	{
		return;
	}

	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToBind);
	
	if (OnInvestigateBehaviourFinishedDelegate.Contains(Delelate) == false)
	{
		OnInvestigateBehaviourFinishedDelegate.AddUnique(Delelate);
	}
}

void ACharacterBaseFrog::UnbindFromInvestigatekBehaviourFinishedDelegate_Implementation(UObject* ListenerObject,
	const FName& FunctionNameToUnBind)
{
	if (!ListenerObject || FunctionNameToUnBind.IsNone())
	{
		return;
	}
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToUnBind);
	
	if (OnInvestigateBehaviourFinishedDelegate.Contains(Delelate) == true)
	{
		OnInvestigateBehaviourFinishedDelegate.Remove(Delelate);
	}
}

void ACharacterBaseFrog::BindToOnReachJumpApexDelegate_Implementation(UObject* ListenerObject,
	const FName& FunctionNameToBind)
{
	if (!ListenerObject || FunctionNameToBind.IsNone())
	{
		return;
	}

	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToBind);
	
	if (OnReachedJumpApex.Contains(Delelate) == false)
	{
		OnReachedJumpApex.AddUnique(Delelate);
	}
}

void ACharacterBaseFrog::UnbindFromReachJumpApexDelegate_Implementation(UObject* ListenerObject,
	const FName& FunctionNameToUnBind)
{
	if (!ListenerObject || FunctionNameToUnBind.IsNone())
	{
		return;
	}
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToUnBind);
	
	if (OnReachedJumpApex.Contains(Delelate) == true)
	{
		OnReachedJumpApex.Remove(Delelate);
	}
}
void ACharacterBaseFrog::BindToLandedDelegate_Implementation(
	UObject* ListenerObject, const FName&FunctionNameToBind)
{
	if (!ListenerObject || FunctionNameToBind.IsNone()) return;
	
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToBind);
	if (LandedDelegate.Contains(Delelate) == false)
	{
		LandedDelegate.AddUnique(Delelate);
	}
}
void ACharacterBaseFrog::UnbindFromLandedDelegate_Implementation(
	UObject* ListenerObject, const FName& FunctionNameToUnBind)
{
	if (!ListenerObject || FunctionNameToUnBind.IsNone()) return;
	
	FScriptDelegate Delelate;
	Delelate.BindUFunction(ListenerObject, FunctionNameToUnBind);
	if (LandedDelegate.Contains(Delelate) == true)
	{
		LandedDelegate.Remove(Delelate);
	}
}

FRotator ACharacterBaseFrog::FindRotationAimedAtFuturePlayerLocation(FVector InAttackLocation)
{
	if (AActor* Actor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector PlayerVelocity = Actor->GetVelocity(); // units cm/s with direction
		FVector PlayerCurrentLocation = Actor->GetActorLocation();

		FVector AttackLocation = PlayerCurrentLocation;
		if (InAttackLocation != FVector::ZeroVector)
		{
			AttackLocation = InAttackLocation;
		}
		
		float DistanceFrogToPlayer = FVector::Dist(PlayerCurrentLocation, this->GetActorLocation()); // units cm

		float TravelTimeFrogToPlayer = DistanceFrogToPlayer / _mSpeedToProjectileOrAttack; // units cm  /  cm/s  = s

		//FVector PlayerFutureLocation = PlayerCurrentLocation + (PlayerVelocity * TravelTimeFrogToPlayer); // units cm + (cm/s x s) = cm
		FVector DifferencePlayerFutureLocation = TravelTimeFrogToPlayer * PlayerVelocity; // units cm + (cm/s x s) = cm
		FVector DistancePlayerCoversWhileTraveling = AttackLocation + (DifferencePlayerFutureLocation * _mPredictionReductionMultiplier); //units cm/s x s = cm

		FRotator AttackRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), DistancePlayerCoversWhileTraveling);
		return AttackRotation;
	}
	return FRotator();
}

void ACharacterBaseFrog::SetSpeedToProjectileOrAttack(float InSpeedToProjectileOrAttack)
{
	_mSpeedToProjectileOrAttack = InSpeedToProjectileOrAttack;
}



float ACharacterBaseFrog::GetDistanceToPlayer()
{
	FVector PlayerLocation;
	if (mPlayerPawn)
	{
		PlayerLocation = mPlayerPawn->GetActorLocation();
		return FVector::Distance(PlayerLocation, GetActorLocation());
	}
	return -1;
}

bool ACharacterBaseFrog::IsInAttackRange()
{
	float DistanceToPlayer = GetDistanceToPlayer();
	bool IsInAttackRange = (DistanceToPlayer >= mAttackRangeMin && DistanceToPlayer <= mAttackRangeMax);
	return IsInAttackRange;
}

float ACharacterBaseFrog::GetDotProductToPlayer()
{
	return FVector::DotProduct(mPlayerPawn->GetActorLocation().GetSafeNormal(), GetActorLocation().GetSafeNormal());
}

bool ACharacterBaseFrog::IsFacingPlayer()
{
	return GetDotProductToPlayer() >= mMinimumDotProductForAttack;
}


//test if AI can attack
void ACharacterBaseFrog::SetIsEngaged(bool InIsEngaged)
{
	_mIsEngaged = InIsEngaged;
}

void ACharacterBaseFrog::StopAttack()
{
	IsAttacking = false;
}




