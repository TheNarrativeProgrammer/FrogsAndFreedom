// Copyright Peasant Pals 2025


#include "Environment/Generator/GeneratorBase.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Hearing.h"


// Sets default values
AGeneratorBase::AGeneratorBase()
{
	//changing from actor to pawn
	//AutoPossessAI = EAutoPossessAI::Disabled;
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponentGenerator"));
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComponent"));
	NoiseEmitterComponent->bAutoActivate = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	BoxComponent =  CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// SceneComponentEnmenyAttackLocationTop = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationTop"));
	// SceneComponentEnmenyAttackLocationBottom = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationBottom"));
	// SceneComponentEnmenyAttackLocationLeft = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationLeft"));
	// SceneComponentEnmenyAttackLocationRight = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationRight"));
	DelegateBindComponentGenerator = CreateDefaultSubobject<UDelegateBindComponentGenerator>(TEXT("DelegateBindComponentGenerator"));
	AttackTokenGiverableComponent = CreateDefaultSubobject<UAttackTokenGiverComponent>(TEXT("AttackTokenGiverComponent"));

	SetRootComponent(StaticMeshComponent);
	//SphereComponent->SetupAttachment(StaticMeshComponent);
	BoxComponent->SetupAttachment(StaticMeshComponent);

	AttackTokenGiverableComponent->SceneComponentEnemyAttackLocationLeftFlusher->SetupAttachment(StaticMeshComponent);
	AttackTokenGiverableComponent->SceneComponentEnemyAttackLocationMidFlusher->SetupAttachment(StaticMeshComponent);
	AttackTokenGiverableComponent->SceneComponentEnemyAttackLocationRightFlusher->SetupAttachment(StaticMeshComponent);
	AttackTokenGiverableComponent->SceneComponentEnemyAttackLocationLeftPinner->SetupAttachment(StaticMeshComponent);
	AttackTokenGiverableComponent->SceneComponentEnemyAttackLocationMidPinner->SetupAttachment(StaticMeshComponent);
	AttackTokenGiverableComponent->SceneComponentEnemyAttackLocationRightPinner->SetupAttachment(StaticMeshComponent);
	// SceneComponentEnmenyAttackLocationTop->SetupAttachment(StaticMeshComponent);
	// SceneComponentEnmenyAttackLocationBottom->SetupAttachment(StaticMeshComponent);
	// SceneComponentEnmenyAttackLocationLeft->SetupAttachment(StaticMeshComponent);
	// SceneComponentEnmenyAttackLocationRight->SetupAttachment(StaticMeshComponent);

	//Populate Array
	

	GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Environment.Generator")));
	TeamId = FGenericTeamId(1);
	
}


//FUNCTIONS - INTERACTABLE
void AGeneratorBase::OnInteract_Implementation()
{
	IInteractable::OnInteract_Implementation();

	if (_mIsForceFieldActive == false)
	{
		if (DelegateBindComponentGenerator->GameStateEventManager)
		{
			SetIsForceFieldActive(true);
			SetSoundTimer();
			DelegateBindComponentGenerator->GameStateEventManager->OnGeneratorInteractSendForceFieldIDDelegate.Broadcast(_mForceFieldID);
			DelegateBindComponentGenerator->GameStateEventManager->OnGeneratorInteractDelegate.Broadcast();
		}
	}
}

void AGeneratorBase::ReverseInteractVisual_Implementation()
{
	IInteractable::ReverseInteractVisual_Implementation();
}

void AGeneratorBase::InteractMessageOn_Implementation(FRotator PlayerRotator)
{
	IInteractable::InteractMessageOn_Implementation(PlayerRotator);
}

void AGeneratorBase::InteractMessageOff_Implementation()
{
	IInteractable::InteractMessageOff_Implementation();
}

// Called when the game starts or when spawned
void AGeneratorBase::BeginPlay()
{
	Super::BeginPlay();
	//moved to Binding delegate on Encounter Start
	

	if (NoiseEmitterComponent->IsActive())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Noise emitter is active"));
	}

	if (ForceFieldClass)
	{
		FActorSpawnParameters SpawnParameters;
		if (AActor* Actor = GetWorld()->SpawnActor<AForceFieldBase>(ForceFieldClass, GetActorLocation(), GetActorRotation(), SpawnParameters))
		{
			SetForceFieldID((Actor->GetUniqueID()));
			//Activate or deactivate ForceField
			if (AForceFieldBase* FoceForceFieldBase = Cast<AForceFieldBase>(Actor))
			{
				FoceForceFieldBase->SetIsActiveOnEncounterBegin(_mIsShieldActiveOnEncounterStart);
			}
			SetIsForceFieldActive(_mIsShieldActiveOnEncounterStart);
		}
		
		//GetWorld()->SpawnActor<AForceFieldBase>(ForceFieldClass, GetActorLocation(), GetActorRotation(), SpawnParameters);
		//RegisterListenersForHealthOfForceFields();
	}
	else
	{
		//LogTemp, Warning, TEXT("No class of force field is set within Generator BP"));
	}

	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationTop);
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationBottom);
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationLeft);
	// PopulateEnemyAttackLocationsArray(SceneComponentEnmenyAttackLocationRight);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	OnBeginplayBroadcastActorInitializationDelegates();
}


void AGeneratorBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}


UHealthComponent* AGeneratorBase::GetHealthComponent()
{
	return HealthComponent;
}


void AGeneratorBase::OnEncounterStartBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnEncounterStartBroadcastActorInitializationDelegates_Implementation();
	if (DelegateBindComponentGenerator->GameStateEventManager)
	{
		DelegateBindComponentGenerator->GameStateEventManager->OnEncounterStartGeneratorInitializeDelegate.Broadcast();
	}
	if (_mIsForceFieldActive == true)
	{
		SetSoundTimer();
	}
}



void AGeneratorBase::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnBeginplayBroadcastActorInitializationDelegates_Implementation();
	if (DelegateBindComponentGenerator->GameStateEventManager)
	{
		DelegateBindComponentGenerator->GameStateEventManager->OnGeneratorInitializedDelegate.Broadcast();
	}
}


// void AGeneratorBase::RegisterListenersForHealthOfForceFields()
// {
// 	ForceField->GetHealthComponent()->OnHealthDepletedDelegate.AddUniqueDynamic(this, &AGeneratorBase::BroadcastHealthDepletedOfForceField);
// }
//
// void AGeneratorBase::BroadcastHealthDepletedOfForceField()
// {
// 	OnHealthDepetedForceFieldDelegate.Broadcast();
// }

// void AGeneratorBase::PopulateEnemyAttackLocationsArray(FVector EnemyAttackLocation)
// {
// 	_maEnemyAttackLocations.Add(EnemyAttackLocation);
// }
//
// TArray<FVector>& AGeneratorBase::GetEnemyAttackLocations()
// {
// 	return _maEnemyAttackLocations;
// }



void AGeneratorBase::SetForceFieldID(int32 InForceFieldID)
{
	_mForceFieldID =InForceFieldID;
}

void AGeneratorBase::SetIsForceFieldActive(bool InIsForceFieldActive)
{
	_mIsForceFieldActive = InIsForceFieldActive;
}

void AGeneratorBase::CheckForceFieldIDMatchesThisOwner(int32 InForceField)
{
	if (InForceField == _mForceFieldID)
	{
		//This future behaviour is specific to the generator 
	}
	//behaviour is agnostic of which generators
	GetWorld()->GetTimerManager().PauseTimer(GeneratorSoundTimerHandle);
	SetIsForceFieldActive(false);
}

bool AGeneratorBase::GetIsForceFieldActive()
{
	return _mIsForceFieldActive;
}





// Called every frame
void AGeneratorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGeneratorBase::OnLeverPullRestoreHealthOfForceField()
{
	SetIsForceFieldActive(true);
}

void AGeneratorBase::SetSoundTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(GeneratorSoundTimerHandle) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(GeneratorSoundTimerHandle, this, &AGeneratorBase::PlayNoise, TimeBetweenNoises, true);
		PlayNoise();
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(GeneratorSoundTimerHandle);
	}
}
void AGeneratorBase::PlayNoise()
{
	FVector Location = GetActorLocation();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), Location, 1.0f,
		Cast<AActor>(this), 0.0f, FName("GeneratorNoise"));
}

int32 AGeneratorBase::GetForceFieldTag()
{
	return _mForceFieldID;
}

void AGeneratorBase::HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint)
{
	AttackTokenGiverableComponent->HandleReturningToken(InTokensRequiredForAction, InIndexOfReservedPoint);
}

bool AGeneratorBase::HandleResevereTokenRequest_Implementation(const FRequesterGeneralTokenData& RequesterTokenData,
	const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData)
{
	
	return AttackTokenGiverableComponent->HandleResevereTokenRequest(RequesterTokenData, RequestedIndexes, ReservationData);
}

FVector AGeneratorBase::HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint)
{
	return AttackTokenGiverableComponent->HandleAttackLocationRequest(InIndexOfReservedPoint);
}

void AGeneratorBase::OnGameOverClearTimer()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


