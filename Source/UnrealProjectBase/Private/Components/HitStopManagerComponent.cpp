// Copyright Peasant Pals 2025


#include "Components/HitStopManagerComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHitStopManagerComponent::UHitStopManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UHitStopManagerComponent::StartHitStop(float Duration, float Dilation)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Dilation);
	
	if (GetWorld()->GetTimerManager().IsTimerActive(EndHitstopTimerHandle) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(EndHitstopTimerHandle, this, &UHitStopManagerComponent::EndHitStop, Duration, false);
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerPaused(EndHitstopTimerHandle) == true)
		{
			GetWorld()->GetTimerManager().UnPauseTimer(EndHitstopTimerHandle);
		}
	}
}


void UHitStopManagerComponent::EndHitStop()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(EndHitstopTimerHandle) == true)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	}
	
}

// Called when the game starts
void UHitStopManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHitStopManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

