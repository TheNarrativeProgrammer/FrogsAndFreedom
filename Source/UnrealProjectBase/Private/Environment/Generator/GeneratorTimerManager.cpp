// Copyright Peasant Pals 2025


#include "Environment/Generator/GeneratorTimerManager.h"

// Sets default values
AGeneratorTimerManager::AGeneratorTimerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DelegateBindingComponentGenTimer = CreateDefaultSubobject<UDelegateBindingComponentGenTimer>(TEXT("DelegateBindingComponentGenTimer"));

}

void AGeneratorTimerManager::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
	IDelegateBindBroadcastable::OnBeginplayBroadcastActorInitializationDelegates_Implementation();
	GetWorldTimerManager().ClearAllTimersForObject(this);
	DelegateBindingComponentGenTimer->GameStateEventManager->OnGeneratorTimerManagerInitializedDelegate.Broadcast();
}



// Called when the game starts or when spawned
void AGeneratorTimerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGeneratorTimerManager::RestartTimer()
{
	GetWorldTimerManager().UnPauseTimer(
		TimerHandleEncounterDurationGeneratorTimer);
	GetWorldTimerManager().UnPauseTimer(
		TimerHandleDelegateBroadcastTimeUpdateGeneratorTimer);
	
}

void AGeneratorTimerManager::PauseTimer()
{
	GetWorldTimerManager().PauseTimer(
		TimerHandleEncounterDurationGeneratorTimer);
	GetWorldTimerManager().PauseTimer(
		TimerHandleDelegateBroadcastTimeUpdateGeneratorTimer);
}

void AGeneratorTimerManager::InitializeAndStartTimer()
{
	//Objective timer
	GetWorldTimerManager().SetTimer(
		TimerHandleEncounterDurationGeneratorTimer, this,
		&AGeneratorTimerManager::BroadcastObjectiveCompleteGeneratorTimer,
		_mTimeLengthObjectiveGeneratorDefence, false);
	
	//UI time update timer
	GetWorldTimerManager().SetTimer(
		TimerHandleDelegateBroadcastTimeUpdateGeneratorTimer, this,
		&AGeneratorTimerManager::BroadcastOnSecondElapsedSendTimeRemainingDelegate,
		_mTimeLengthBroadcastTimeElaspedDelegate, true);

	if (DelegateBindingComponentGenTimer)
	{
		if (DelegateBindingComponentGenTimer->GameStateEventManager)
		{
			DelegateBindingComponentGenTimer->GameStateEventManager->
				OnGeneratorTimerManagerTimerStarSendTotalTimeDelegate.Broadcast(
					_mTimeLengthObjectiveGeneratorDefence);
			
			DelegateBindingComponentGenTimer->GameStateEventManager->
				OnGeneratorTimerManagerTimerStartDelegate.Broadcast();
		}
	}
}

void AGeneratorTimerManager::TurnPowerOn()
{
	if (DelegateBindingComponentGenTimer)
	{
		if (DelegateBindingComponentGenTimer->GameStateEventManager)
		{
			DelegateBindingComponentGenTimer->GameStateEventManager->
				OnAllForceFieldActivatedTurnPowerOnDelegate.Broadcast();
		}
	}
	
}

void AGeneratorTimerManager::TurnPowerOff()
{
	if (DelegateBindingComponentGenTimer)
	{
		if (DelegateBindingComponentGenTimer->GameStateEventManager)
		{
			DelegateBindingComponentGenTimer->GameStateEventManager->
				OnForceFieldDeactivatedTurnPowerOffDelegate.Broadcast();
		}
	}
}

void AGeneratorTimerManager::BroadcastOnSecondElapsedSendTimeRemainingDelegate()
{
	if (DelegateBindingComponentGenTimer->GameStateEventManager)
	{
		DelegateBindingComponentGenTimer->GameStateEventManager->
			OnGeneratorTimerManagerSecondElaspedDelegate.Broadcast(GetRemainingTime());
	}
}

void AGeneratorTimerManager::BroadcastObjectiveCompleteGeneratorTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandleEncounterDurationGeneratorTimer);
	GetWorldTimerManager().ClearTimer(TimerHandleDelegateBroadcastTimeUpdateGeneratorTimer);
	if (DelegateBindingComponentGenTimer->GameStateEventManager)
	{
		DelegateBindingComponentGenTimer->GameStateEventManager->
			OnGeneratorTimerManagerTimerFinishedDelegate.Broadcast();
	}
}


bool AGeneratorTimerManager::GetIsTimerInitialized_Implementation()
{
	bool isPausedAndExist = GetWorldTimerManager().IsTimerPaused(
		TimerHandleEncounterDurationGeneratorTimer);

	return  isPausedAndExist;
}


float AGeneratorTimerManager::GetElaspedTime() const
{
	if (GetWorldTimerManager().TimerExists(TimerHandleEncounterDurationGeneratorTimer))
	{
		float TimeElasped = GetWorldTimerManager().GetTimerElapsed(TimerHandleEncounterDurationGeneratorTimer);
		return TimeElasped;
	}
	else
	{
		return 0;
	}
}

float AGeneratorTimerManager::GetRemainingTime() const
{
	if (_mTimeLengthObjectiveGeneratorDefence <= 0.0f)
	{
		return 0.0f;
	}
	float TimeElasped = GetElaspedTime();
	float TimeRemaining = FMath::Clamp(
		_mTimeLengthObjectiveGeneratorDefence - TimeElasped,
		0.0f, _mTimeLengthObjectiveGeneratorDefence);
	
	if (GetWorldTimerManager().IsTimerActive(
		TimerHandleEncounterDurationGeneratorTimer) == false)
	{
		return 0.0f;
	}
	return TimeRemaining;
}

void AGeneratorTimerManager::OnGameOverClearTimer()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

