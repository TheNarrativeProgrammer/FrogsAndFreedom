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






void AComputerGeneratorActivator::OnEncounterStartBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnEncounterStartBroadcastActorInitializationDelegates_Implementation();
	
}

void AComputerGeneratorActivator::SetLeverState(TEnumAsByte<eLeverState> LeverState)
{
	_meLeverState = LeverState;
}



void AComputerGeneratorActivator::OnHealthDepletedOfGeneratorForcceFieldChangeComputerState()
{
	SetLeverState(eLeverState::UpAndPullable);
	Execute_ReverseInteractVisual(this);
	PauseTimer();
}



void AComputerGeneratorActivator::OnLeverPullRestoreForceField()
{
}


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
}

void AComputerGeneratorActivator::BroadcastOnSecondElaspedSendTimeRemainingDelegate()
{
}

float AComputerGeneratorActivator::GetElaspedTime() const
{
	if (GetWorldTimerManager().TimerExists(TimerHandleEncounterDuration))
	{
		float TimeElasped = GetWorldTimerManager().GetTimerElapsed(TimerHandleEncounterDuration);
		return TimeElasped;
	}
	else
	{
		return 0;
	}
	
}

float AComputerGeneratorActivator::GetRemainingTime() const
{
	if (_mTimeLengthUntilDoorOpens <= 0.0f)
	{
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

