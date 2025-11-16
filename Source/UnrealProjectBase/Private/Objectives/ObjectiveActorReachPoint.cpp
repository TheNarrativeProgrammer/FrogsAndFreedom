// Copyright Peasant Pals 2025


#include "Objectives/ObjectiveActorReachPoint.h"

#include "Characters/Player/CharacterBasePlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AObjectiveActorReachPoint::AObjectiveActorReachPoint()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
}

void AObjectiveActorReachPoint::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this,
		&AObjectiveActorReachPoint::OnOverlapObjectiveComplete);
}

bool AObjectiveActorReachPoint::IsObjectiveCompleted_Implementation() const
{
	if (_mIsAutoCompleteObjective == true) return true;
	
	if (_mHasPlayerReachedPoint == true && Super::IsObjectiveCompleted_Implementation())
	{
		return true;
	}
	return false;
}

void AObjectiveActorReachPoint::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	if (AGameStateEventManagerBase* GameStateEventManager =
		Cast<AGameStateEventManagerBase>
		(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager->OnActorStateChangeUpdateUIDelegate.Broadcast();
	}
}

void AObjectiveActorReachPoint::CompleteObjectiveBroadcast_Implementation()
{
	Super::CompleteObjectiveBroadcast_Implementation();
}

void AObjectiveActorReachPoint::SetHasPlayerReachedPoint(bool InHasPlayerReachedPoint)
{
	_mHasPlayerReachedPoint = InHasPlayerReachedPoint;
}

void AObjectiveActorReachPoint::OnOverlapObjectiveComplete_Implementation(
	UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,
	bool bFromSweep,const FHitResult& SweepResult)
{
		if (OtherActor->Implements<UPlayerCharacterable>()
			&& _mHasObjectiveStarted == true)
		{
			TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this,
				&AObjectiveActorReachPoint::OnOverlapObjectiveComplete);
			
			SetHasPlayerReachedPoint(true);
			IfCompleteTriggerSubObjectivesAndWave();
		}
}
