// Copyright Peasant Pals 2025


#include "Objectives/ObjectiveActorTurnOnAllGenerator.h"
#include "Components/BindComponents/DelegateBindingComponentObjGenOn.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Text.h"
#include "Environment/Generator/GeneratorBase.h"


AObjectiveActorTurnOnAllGenerator::AObjectiveActorTurnOnAllGenerator()
{
	DelegateBindingComponentObjGenOn = CreateDefaultSubobject<
		UDelegateBindingComponentObjGenOn>(TEXT("DelegateBindingComponent"));
}

void AObjectiveActorTurnOnAllGenerator::SetIsFirstIterationComplete()
{
	_IsFirstIterationComplete = true;
}

void AObjectiveActorTurnOnAllGenerator::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	if (DelegateBindingComponentObjGenOn)
	{
		if (DelegateBindingComponentObjGenOn->GameStateEventManager)
		{
			DelegateBindingComponentObjGenOn->GameStateEventManager->
				OnActorStateChangeUpdateUIDelegate.Broadcast();
		}
	}
}



void AObjectiveActorTurnOnAllGenerator::CompleteObjectiveBroadcast_Implementation()
{
	Super::CompleteObjectiveBroadcast_Implementation();
}


void AObjectiveActorTurnOnAllGenerator::BroadcastUIUpdateEvent()
{
	DelegateBindingComponentObjGenOn->GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
}

void AObjectiveActorTurnOnAllGenerator::BroadcastCompletionReverseredEvent()
{
	if (_mIsTimerFinished == false)
	{
		int CountActiveForceFields = 0;
		for (AGeneratorBase* GeneratorBase : _msGeneratorActorsInLevel)
		{
			if (!GeneratorBase) continue;
		
			if (GeneratorBase->GetIsForceFieldActive() == true)
			{
				CountActiveForceFields++;
			}
		}
		if (CountActiveForceFields == _msGeneratorActorsInLevel.Num() - 1)
		{
			SetIsFreed(false);
			SetIsCompletionTextVisible(true);
			StopSubObjectives();
			SetIsFirstIterationComplete();
			DelegateBindingComponentObjGenOn->GameStateEventManager->
				OnActorObjectiveCompletionReversedUpdateUIDelegate.Broadcast(
					this);
		}
	}
}

bool AObjectiveActorTurnOnAllGenerator::IsObjectiveCompleted_Implementation() const
{
	if (Super::IsObjectiveCompleted_Implementation() == false) return false;
	
	if (_mIsAutoCompleteObjective == true || _mIsTimerFinished == true)
	{
		return true;
	}
	
	for (AGeneratorBase* GeneratorBase : _msGeneratorActorsInLevel)
	{
		if (!GeneratorBase) continue;
		
		if (GeneratorBase->GetIsForceFieldActive() == false)
		{
			return false;
		}
	}
	return true;
}


FText AObjectiveActorTurnOnAllGenerator::GetObjectiveDisplayText_Implementation() const
{
	int ActiveShields = 0;
	for (AGeneratorBase* GeneratorBase : _msGeneratorActorsInLevel)
	{
		if (!GeneratorBase)continue;
		
		if (GeneratorBase->GetIsForceFieldActive() == true)
		{
			ActiveShields++;
		}
	}

	if (_IsFirstIterationComplete == false)
	{
		#define LOCTEXT_NAMESPACE "ObjectivesNameSpace"
				FFormatNamedArguments Args;
				Args.Add(TEXT("DisplayTextKey"), _mObjectiveDisplayName);
				Args.Add(TEXT("ActiveShieldsKey"), FText::AsNumber(ActiveShields));
				Args.Add(TEXT("TotalGeneratorsKey"), FText::AsNumber(_msGeneratorActorsInLevel.Num()));
				const FText GeneratorShieldFormat = LOCTEXT(
					"GeneratorShieldProgressFormat", "{DisplayTextKey} : {ActiveShieldsKey} / {TotalGeneratorsKey}");

				FText OutputText = FText::Format(GeneratorShieldFormat, Args);
				return OutputText;
		#undef LOCTEXT_NAMESPACE
	}
	else
	{
		#define LOCTEXT_NAMESPACE "AltNameSpace"
				FText CombineInfoText = FText::Format(
					LOCTEXT("GeneratorSheildFormat", "{0} Active Shields : {1} / {2}"),
					_mObjectiveDisplayNameSecondIteration,
					FText::AsNumber(ActiveShields),
					FText::AsNumber(_msGeneratorActorsInLevel.Num())
				);
		#undef LOCTEXT_NAMESPACE
				return CombineInfoText;
	}
	
	return Super::GetObjectiveDisplayText_Implementation();
}

void AObjectiveActorTurnOnAllGenerator::SetIsTimerFinished(
	bool InIsTimerFinished)
{
	_mIsTimerFinished = InIsTimerFinished;
}

void AObjectiveActorTurnOnAllGenerator::OnTimerFinishedSetToTrue()
{
	if (_mIsTimerFinished == false)
	{
		SetIsCompletionTextVisible(false);
		SetIsTimerFinished(true);
	}
}

void AObjectiveActorTurnOnAllGenerator::BeginPlay()
{
	Super::BeginPlay();
	SetIsTimerFinished(false);
	_IsFirstIterationComplete = false;
}
