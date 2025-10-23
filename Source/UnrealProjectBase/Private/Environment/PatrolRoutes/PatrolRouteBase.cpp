// Copyright Peasant Pals 2025


#include "Environment/PatrolRoutes/PatrolRouteBase.h"

// Sets default values
APatrolRouteBase::APatrolRouteBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpineComponentPatrolRoute = CreateDefaultSubobject<USplineComponent>("SpineComponentPatrolRoute");
	SpineComponentPatrolRoute->SetupAttachment(GetRootComponent());

	//GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Environment.PatrolRoute.Type.Pinner")));

}

void APatrolRouteBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}

void APatrolRouteBase::IncrumentPatrolRoute(int32& FrogPatrolIndex, int32& FrogDirection)
{
	//incrument index by the current direction, either 1 or -1.
	FrogPatrolIndex = FrogPatrolIndex + FrogDirection;
	
	int32 SpinePointQuantity = SpineComponentPatrolRoute->GetNumberOfSplinePoints() - 1;
	if (FrogPatrolIndex >= SpinePointQuantity)
	{
		//reverse direction from forward to Backward if index is at the end
		FrogDirection = -1; 
	}
	else
	{
		if (FrogPatrolIndex <= 0)
		{
			//reverse direction from backward to Forward if index is at the beginning
			FrogDirection = 1;
		}
	}
}

FVector APatrolRouteBase::GetSplinePointAsWorldLocation(const int32& FrogPatrolIndex)
{
	return SpineComponentPatrolRoute->GetLocationAtSplinePoint(FrogPatrolIndex, ESplineCoordinateSpace::World);
}

// Called when the game starts or when spawned
void APatrolRouteBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolRouteBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

