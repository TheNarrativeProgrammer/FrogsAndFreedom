// Copyright Peasant Pals 2025


#include "Objectives/ObjectiveActorDefendGenTime.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/Generator/GeneratorTimerManager.h"
#include"Environment/Generator/ForceFieldBase.h"

AObjectiveActorDefendGenTime::AObjectiveActorDefendGenTime()
{
	DelegateBindingComponentGenTimer = CreateDefaultSubobject<UDelegateBindingComponentObjTimer>(TEXT("DelegateBindingComponentGenTimer"));
}

void AObjectiveActorDefendGenTime::BroadcastUIUpdateEvent()
{
}

bool AObjectiveActorDefendGenTime::IsObjectiveCompleted_Implementation() const
{
	if (Super::IsObjectiveCompleted_Implementation() == false) return false;
	
	if (_mIsAutoCompleteObjective == true)
	{
		return true;
	}
	if (_mIsTimerFinished == false)
	{
		return false;
	}
	return true;
}

FText AObjectiveActorDefendGenTime::GetObjectiveDisplayText_Implementation() const
{
	#define LOCTEXT_NAMESPACE "ObjectiveActorDefendGenTime"
			int TimeRemainingAtInt = std::clamp(static_cast<int>(_mTimeRemaining), 0, 240);
			FFormatNamedArguments Args;
			Args.Add(TEXT("DisplayTextKey"), _mObjectiveDisplayName);
			Args.Add(TEXT("TimeRemainingKey"), FText::AsNumber(TimeRemainingAtInt));

			const FText DefendGeneratorFormat = LOCTEXT(
				"DefendGeneratorProgessFormat", "{DisplayTextKey} : {TimeRemainingKey}");

			FText OutputText = FText::Format(DefendGeneratorFormat, Args);

			return OutputText;
	#undef LOCTEXT_NAMESPACE

}

void AObjectiveActorDefendGenTime::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	if (DelegateBindingComponentGenTimer)
	{
		if (DelegateBindingComponentGenTimer->GameStateEventManager)
		{
			DelegateBindingComponentGenTimer->GameStateEventManager->
				OnTimerChangeUpdateUIDelegate.Broadcast(this);
		}
		if (GeneratorTimerManager)
		{
			if (GeneratorTimerManager->GetIsTimerInitialized() == false)
			{
				GeneratorTimerManager->InitializeAndStartTimer();
			}
			else
			{
				GeneratorTimerManager->RestartTimer();
			}
			GeneratorTimerManager->TurnPowerOn();
		}
	}
}

void AObjectiveActorDefendGenTime::StopObjective_Implementation()
{
	if (IsObjectiveStarted() == true && _mIsTimerFinished == false)
	{
		Super::StopObjective_Implementation();
		if (GeneratorTimerManager)
		{
			GeneratorTimerManager->PauseTimer();
			GeneratorTimerManager->TurnPowerOff();
		}
	}
}


void AObjectiveActorDefendGenTime::CompleteObjectiveBroadcast_Implementation()
{
	Super::CompleteObjectiveBroadcast_Implementation();
}

void AObjectiveActorDefendGenTime::OnTimerFinishedCompleteObjective()
{
	_mIsTimerFinished = true;
	if (SubObjectives.IsEmpty() == false)
	{
		if (DelegateBindingComponentGenTimer)
		{
			if (DelegateBindingComponentGenTimer->GameStateEventManager)
			{
				SetIsCompletionTextVisible(true);
				DelegateBindingComponentGenTimer->GameStateEventManager->
					OnActorObjectiveCompleteUpdateUIDelegate.Broadcast();
			}
		}
	}
	IfCompleteTriggerSubObjectivesAndWave();
}

void AObjectiveActorDefendGenTime::UpdateTimeRemainingAndBroadcastToUI(
	float InTimeRemaning)
{
	_mTimeRemaining = InTimeRemaning;
	if (DelegateBindingComponentGenTimer)
	{
		if (DelegateBindingComponentGenTimer->GameStateEventManager)
		{
			DelegateBindingComponentGenTimer->GameStateEventManager->
				OnTimerChangeUpdateUIDelegate.Broadcast(this);
		}
	}
}
