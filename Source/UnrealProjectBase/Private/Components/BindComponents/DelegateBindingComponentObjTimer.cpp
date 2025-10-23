// Copyright Peasant Pals 2025

#include "Components/BindComponents/DelegateBindingComponentObjTimer.h"
#include "Objectives/ObjectiveActorDefendGenTime.h"



void UDelegateBindingComponentObjTimer::BindDelegateFromBP()
{
	Super::BindDelegateFromBP();
}

void UDelegateBindingComponentObjTimer::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (AObjectiveActorDefendGenTime* ObjectiveActorDefendGenTimeOwner =
			Cast<AObjectiveActorDefendGenTime>(GetOwner()))
		{
			GameStateEventManager->OnGeneratorTimerManagerSecondElaspedDelegate.AddUniqueDynamic(
				ObjectiveActorDefendGenTimeOwner, &AObjectiveActorDefendGenTime::UpdateTimeRemainingAndBroadcastToUI);
			
			GameStateEventManager->OnGeneratorTimerManagerTimerFinishedDelegate.AddUniqueDynamic(
				ObjectiveActorDefendGenTimeOwner, &AObjectiveActorDefendGenTime::SetIsTimerFinished);
		}
	}
}
