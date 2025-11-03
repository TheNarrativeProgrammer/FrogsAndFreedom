// Copyright Peasant Pals 2025

#include "Environment/Generator/ComputerGeneratorActivator.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
AComputerGeneratorActivator::AComputerGeneratorActivator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponentComputer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentComputer"));
	StaticMeshComponentLever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentLever"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	DelegateBindComponentComputer = CreateDefaultSubobject<UDelegateBindComponentComputer>(TEXT("DelegateBindComponentComputer"));

	SetRootComponent(StaticMeshComponentComputer);
	BoxComponent->SetupAttachment(StaticMeshComponentComputer);
	StaticMeshComponentLever->SetupAttachment(StaticMeshComponentComputer);
}

// Called when the game starts or when spawned
void AComputerGeneratorActivator::BeginPlay()
{
	Super::BeginPlay();
	OnBeginplayBroadcastActorInitializationDelegates();
}

void AComputerGeneratorActivator::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnBeginplayBroadcastActorInitializationDelegates_Implementation();
	// DelegateBindComponentComputer->GameStateEventManager->OnComputerGeneratorActivatorInitializedDelegate.Broadcast();
}




void AComputerGeneratorActivator::ReverseInteractVisual_Implementation()
{
	//BP function triggers timeline reversing animation. Lever Now Up
	//LogTemp, Warning, TEXT("ReverseInteractVisual_Implementation from ComputerGeneratorActivator"));
	
	if (_meLeverState == eLeverState::DownAndNotPullable)
	{
		SetLeverState(eLeverState::UpAndPullable);
	}
}



void AComputerGeneratorActivator::OnInteract_Implementation()
{
	//BP function triggers timeline forward animation. Lever Now Down
	//UE_LOG(LogTemp, Warning, TEXT("OnInteract_Implementation from ComputerGeneratorActivator"));
	
	if (_meLeverState == eLeverState::UpAndPullable)
	{
		SetLeverState(eLeverState::DownAndNotPullable);
		OnLeverPullRestoreForceField();
		//UpdateGeneratorData();
	}

	//check if timer is paused - if true then unpause
	if (GetWorldTimerManager().IsTimerPaused(TimerHandleEncounterDuration) == true)
	{
		RestartTimer();
	}
	//check if timer is active - if inactive, then start/initialize timer
	else if (GetWorldTimerManager().IsTimerActive(TimerHandleEncounterDuration) == false)
	{
		InitializeAndStartTimer();
	}
}





// void AComputerGeneratorActivator::RegisterListenersForHealthOfGeneratorForceField(AGeneratorBase* GeneratorBase)
// {
// 	GeneratorBase->OnHealthDepetedForceFieldDelegate.AddUniqueDynamic(this, &AComputerGeneratorActivator::OnHealthDepletedOfGeneratorForcceFieldChangeComputerState);
// }


// void AComputerGeneratorActivator::PopulateArrayWithActorsOfClass()
// {
// 	TArray<AActor*> FoundActors;
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeneratorBase::StaticClass(), FoundActors);
//
// 	_maGeneratorData.Empty();
//
// 	if (FoundActors.Num() <= 0)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Zero Generators are in level. Place at least 1 to use Computer"));
// 	}
//
// 	for (AActor* Actor : FoundActors)
// 	{
// 		FGeneratorData GeneratorData;
// 		AGeneratorBase* GeneratorBase = Cast<AGeneratorBase>(Actor);
// 		if (GeneratorBase)
// 		{
// 			
// 			GeneratorData.GeneratorBaseActor = GeneratorBase;
// 			GeneratorData.IsForceFieldActive = _mIsGeneratorForceFieldStartingActive;
// 			GeneratorData.IsGeneratorHealthDepleted = false;
// 			//RegisterListenersForHealthOfGeneratorForceField(GeneratorBase);
// 		}
// 		_maGeneratorData.Add(GeneratorData);
// 	}
// }

// void AComputerGeneratorActivator::RemoveNullReferencesFromPreviousLevels()
// {
// 	TArray<FGeneratorData> AGeneratorDataWithPossibleNullReferences = _maGeneratorData;
// 	int32 ArrayIndex = 0;
// 	for (FGeneratorData& GeneratorData : AGeneratorDataWithPossibleNullReferences)
// 	{
// 		if (IsValid(GeneratorData.GeneratorBaseActor) == false)
// 		{
// 			_maGeneratorData.RemoveAt(ArrayIndex);
// 			ArrayIndex++;
// 		}
// 	}
// }

void AComputerGeneratorActivator::OnEncounterStartBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnEncounterStartBroadcastActorInitializationDelegates_Implementation();
	if (DelegateBindComponentComputer->GameStateEventManager)
	{
		//DelegateBindComponentComputer->GameStateEventManager->OnEncounterStartComputerGeneratorActivatorInitializeDelegate.Broadcast();
	}
	
}

void AComputerGeneratorActivator::SetLeverState(TEnumAsByte<eLeverState> LeverState)
{
	_meLeverState = LeverState;
}



void AComputerGeneratorActivator::OnHealthDepletedOfGeneratorForcceFieldChangeComputerState()
{
	SetLeverState(eLeverState::UpAndPullable);
	//ReverseInteractVisual();
	Execute_ReverseInteractVisual(this);
	PauseTimer();
	//UpdateGeneratorData();
}



void AComputerGeneratorActivator::OnLeverPullRestoreForceField()
{
	// if (DelegateBindComponentComputer->GameStateEventManager)
	// {
	// 	DelegateBindComponentComputer->GameStateEventManager->OnComputerGeneratorLeverPullDelegate.Broadcast();
	// }
}

// void AComputerGeneratorActivator::UpdateGeneratorData()
// {
// 	for (FGeneratorData& GeneratorData : _maGeneratorData)
// 	{
// 		GeneratorData.IsForceFieldActive = GeneratorData.GeneratorBaseActor->GetForceField()->GetIsForceFieldActive();
// 		GeneratorData.IsGeneratorHealthDepleted = GeneratorData.GeneratorBaseActor->GetHealthComponent()->IsHealthDepleted();
// 	}
// }

void AComputerGeneratorActivator::InitializeAndStartTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandleEncounterDuration, this, &AComputerGeneratorActivator::OpenDoorToNextLevel, _mTimeLengthUntilDoorOpens, false);
	GetWorldTimerManager().SetTimer(TimerHandleDelegateBroadcastTimeUpdate, this, &AComputerGeneratorActivator::BroadcastOnSecondElaspedSendTimeRemainingDelegate, _mTimeLengthBroadcastTimeElaspedDelegate, true);
}

void AComputerGeneratorActivator::PauseTimer()
{
	GetWorldTimerManager().PauseTimer(TimerHandleEncounterDuration);
}

void AComputerGeneratorActivator::RestartTimer()
{
	if (GetWorldTimerManager().IsTimerPaused(TimerHandleEncounterDuration) == true)
	{
		GetWorldTimerManager().UnPauseTimer(TimerHandleEncounterDuration);
	}
}

void AComputerGeneratorActivator::OpenDoorToNextLevel()
{
	GetWorldTimerManager().ClearTimer(TimerHandleEncounterDuration);
	GetWorldTimerManager().ClearTimer(TimerHandleDelegateBroadcastTimeUpdate);

	// if (DelegateBindComponentComputer->GameStateEventManager)
	// {
	// 	DelegateBindComponentComputer->GameStateEventManager->OnComputerGeneratorActivatorTimerFinishedDelegate.Broadcast();
	// }
	
	//OnGeneratorTimerFinishedDelegate.Broadcast();
}

void AComputerGeneratorActivator::BroadcastOnSecondElaspedSendTimeRemainingDelegate()
{
	//DelegateBindComponentComputer->GameStateEventManager->OnComputerGeneratorActivatorSecondElaspedDelegate.Broadcast(GetRemainingTime());
}

float AComputerGeneratorActivator::GetElaspedTime() const
{
	if (GetWorldTimerManager().TimerExists(TimerHandleEncounterDuration))
	{
		float TimeElasped = GetWorldTimerManager().GetTimerElapsed(TimerHandleEncounterDuration);
		//UE_LOG(LogTemp, Warning, TEXT("Timer Is set"));
		return TimeElasped;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Timer doesn't exist within Computer Generator Activator"));
		return 0;
	}
	
}

float AComputerGeneratorActivator::GetRemainingTime() const
{
	if (_mTimeLengthUntilDoorOpens <= 0.0f)
	{
		//
		//
		//UE_LOG(LogTemp, Warning, TEXT("TimeLengthUntilDoorOpens <= 0.0. Set time in Computer Gernator"));
		return 0.0f;
	}
	float TimeElasped = GetElaspedTime();
	float TimeRemaining = FMath::Clamp(_mTimeLengthUntilDoorOpens - TimeElasped, 0.0f, _mTimeLengthUntilDoorOpens);
	if (GetWorldTimerManager().IsTimerActive(TimerHandleEncounterDuration) == false)
	{
		return 0.0f;
	}
	return TimeRemaining;
}

// Called every frame
void AComputerGeneratorActivator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

