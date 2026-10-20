// Copyright Peasant Pals 2025


#include "Environment/Generator/ForceFieldBase.h"



// Sets default values
AForceFieldBase::AForceFieldBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//Create default subobjects
	SceneComponentRoot = CreateDefaultSubobject<USceneComponent>("SceneComponentRo");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("SphereComponentForceField");
	// NiagaraComponentForceField = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponentForceField");
	
	NiagaraComponentCracks = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponentCracks");
	DelegateBindComponentForceField = CreateDefaultSubobject<UDelegateBindComponentForceField>("DelegateBindComponentForceField");

	// SceneComponentEnmenyAttackLocationTop = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationTop"));
	// SceneComponentEnmenyAttackLocationBottom = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationBottom"));
	// SceneComponentEnmenyAttackLocationLeft = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationLeft"));
	// SceneComponentEnmenyAttackLocationRight = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationRight"));

	AttackTokenGiverableComponent = CreateDefaultSubobject<UAttackTokenGiverComponent>(TEXT("AttackTokenGiverComponent"));
	

	//Set root and attachments
	SetRootComponent(SceneComponentRoot);
	CapsuleComponent->SetupAttachment(SceneComponentRoot);
	// NiagaraComponentForceField->SetupAttachment(SceneComponentRoot);
	NiagaraComponentCracks->SetupAttachment(SceneComponentRoot);

	// SceneComponentEnmenyAttackLocationTop->SetupAttachment(SceneComponentRoot);
	// SceneComponentEnmenyAttackLocationBottom->SetupAttachment(SceneComponentRoot);
	// SceneComponentEnmenyAttackLocationLeft->SetupAttachment(SceneComponentRoot);
	// SceneComponentEnmenyAttackLocationRight->SetupAttachment(SceneComponentRoot);

	//Populate Array
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationTop);
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationBottom);
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationLeft);
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationRight);

	GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Environment.Generator")));
	TeamId = FGenericTeamId(1);

}

void AForceFieldBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}

void AForceFieldBase::BeginPlay()
{
	Super::BeginPlay();
	_mForceFieldID = GetUniqueID();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChangedDelegate.AddUniqueDynamic(
		this, &AForceFieldBase::OnHealthChangeSendIdAndHealth);
	
		HealthComponent->OnHealthDepletedDelegate.AddUniqueDynamic(
			this, &AForceFieldBase::Death);
	}
}

void AForceFieldBase::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnBeginplayBroadcastActorInitializationDelegates_Implementation();
}

void AForceFieldBase::Death()
{
	DeactivateComponents();
	DelegateBindComponentForceField->GameStateEventManager->OnForceFieldBaseDeathIDDelegate.Broadcast(GetUniqueID());
	DelegateBindComponentForceField->GameStateEventManager->FOnForceFieldDeathDelegate.Broadcast();
}




void AForceFieldBase::OnEncounterStartBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnEncounterStartBroadcastActorInitializationDelegates_Implementation();
}



void AForceFieldBase::OnHealthChangeSendIdAndHealth(
	float NewHealth, float OldHealth)
{
	if (DelegateBindComponentForceField)
	{
		if (DelegateBindComponentForceField->GameStateEventManager)
		{
			DelegateBindComponentForceField->GameStateEventManager->
				OnForceFieldHealthChangeIDHealthDelegate.Broadcast(
				_mForceFieldID, HealthComponent->GetCurrentHealth(),
				HealthComponent->GetMaxHealth());
		}
	}
}

void AForceFieldBase::RestoreHealth()
{
	if (HealthComponent)
	{
		HealthComponent->ResetHealthToDefault();
	}
}


void AForceFieldBase::DeactivateComponents_Implementation()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetIsForceFieldActive(false);
}

void AForceFieldBase::ActivateComponents_Implementation()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetIsForceFieldActive(true);
}


void AForceFieldBase::OnGeneratorInteractRestoreHealthAndActivateIfIdMatches_Implementation(int32 ActorID)
{
	if (ActorID == GetUniqueID())
	{
		RestoreHealth();
		ActivateComponents();
	}
}

void AForceFieldBase::SetIsForceFieldActive(bool IsForceFieldActive)
{
	_mIsForceFieldActive = IsForceFieldActive;
}

bool AForceFieldBase::GetIsForceFieldActive()
{
	return _mIsForceFieldActive;
}


void AForceFieldBase::SetIsActiveOnEncounterBegin(bool InIsActiveOnEncounterBegin)
{
	if (InIsActiveOnEncounterBegin == true)
	{
		ActivateComponents();
	}
	else
	{
		DeactivateComponents();
	}
	
}

void AForceFieldBase::HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint)
{
	AttackTokenGiverableComponent->HandleReturningToken(InTokensRequiredForAction, InIndexOfReservedPoint);
}

bool AForceFieldBase::HandleResevereTokenRequest_Implementation(const FRequesterGeneralTokenData& RequesterTokenData,
	const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData)
{
	return AttackTokenGiverableComponent->HandleResevereTokenRequest(RequesterTokenData, RequestedIndexes, ReservationData);
}

FVector AForceFieldBase::HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint)
{
	return AttackTokenGiverableComponent->HandleAttackLocationRequest(InIndexOfReservedPoint);
}

void AForceFieldBase::OnEncounterEndDestroyThis_Implementation()
{
	if (DelegateBindComponentForceField)
	{
		DelegateBindComponentForceField->UnBindDelegates();
	}
	Destroy();
}




UHealthComponent* AForceFieldBase::GetHealthComponent()
{
	return HealthComponent;
}

