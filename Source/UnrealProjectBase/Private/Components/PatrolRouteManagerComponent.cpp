// Copyright Peasant Pals 2025


#include "Components/PatrolRouteManagerComponent.h"

#include "Environment/ActorSpawner/EncounterData.h"

// Sets default values for this component's properties
UPatrolRouteManagerComponent::UPatrolRouteManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UPatrolRouteManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	GameplayTagFlusher = FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Flusher");
	GameplayTagPinner = FGameplayTag::RequestGameplayTag("Character.Enemy.Type.Pinner");

	// ...
	
}


void UPatrolRouteManagerComponent::RunEqsFindPatrolRoutes_Implementation()
{
}

// Called every frame
void UPatrolRouteManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPatrolRouteManagerComponent::SetRadiusOfEQSBaseOnEncounterSize(float inRadiusOfEQSBaseOnEncounterSize)
{
	_mRadiusOfEQSBaseOnEncounterSize = inRadiusOfEQSBaseOnEncounterSize;
}

void UPatrolRouteManagerComponent::AddEncounterPatrolRoutesToArrays(AActor* Actor)
{
	if (APatrolRouteBase* PatrolRouteBase = Cast<APatrolRouteBase>(Actor))
	{
		FGameplayTagContainer GameplayTagContainer = PatrolRouteBase->GameplayTagContainer;
		if (GameplayTagContainer.IsValid())
		{
			if (GameplayTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Environment.PatrolRoute.Type.Flusher"))))
			{
				_maPatrolRoutesFlusher.AddUnique(PatrolRouteBase);
			}
			if (GameplayTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Environment.PatrolRoute.Type.Pinner"))))
			{
				_maPatrolRoutesPinner.AddUnique(PatrolRouteBase);
			}
		}
	}
}

void UPatrolRouteManagerComponent::EmptyArraysPatrolRoutes()
{
	_maPatrolRoutesFlusher.Empty();
	_maPatrolRoutesPinner.Empty();
}

void UPatrolRouteManagerComponent::SetPatrolRouteOfBatch(const FEncounterSpawnData& EncounterSpawnData,
                                                         int32 InIndexOfPatrolRoute, APatrolRouteBase*& InPatrolRouteBaseOfCurrentBatch)
{
	if (EncounterSpawnData.GameplayTagFrog.MatchesTag(GameplayTagFlusher))
	{
		if (InIndexOfPatrolRoute >= _maPatrolRoutesFlusher.Num() && _maPatrolRoutesFlusher.IsEmpty() == false)
		{
			InIndexOfPatrolRoute = InIndexOfPatrolRoute % _maPatrolRoutesFlusher.Num();
		}
		if (_maPatrolRoutesFlusher.IsEmpty() == false)
		{
			InPatrolRouteBaseOfCurrentBatch = _maPatrolRoutesFlusher[InIndexOfPatrolRoute];
		}
		else
		{
			//UE_LOG(LogTemp, Warning , TEXT("DESIGNER: There are no patrol routes with the Flusher tag."));
		}
	}
	
	if (EncounterSpawnData.GameplayTagFrog.MatchesTag(GameplayTagPinner))
	{
		if (InIndexOfPatrolRoute >= _maPatrolRoutesPinner.Num() && _maPatrolRoutesPinner.IsEmpty() == false)
		{
			InIndexOfPatrolRoute = InIndexOfPatrolRoute % _maPatrolRoutesPinner.Num();
		}
		if (_maPatrolRoutesPinner.IsEmpty() == false)
		{
			InPatrolRouteBaseOfCurrentBatch = _maPatrolRoutesPinner[InIndexOfPatrolRoute];
		}
		else
		{
			//UE_LOG(LogTemp, Warning , TEXT("DESIGNER: There are no patrol routes with the Pinner tag."));
		}
	}
}





