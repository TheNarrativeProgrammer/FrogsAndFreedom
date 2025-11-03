// Copyright Peasant Pals 2025


#include "Components/DelegateBindingComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDelegateBindingComponent::UDelegateBindingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UDelegateBindingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>(
		UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
	}
	BindDelegateFromBP();
	BindDelegates();
}

void UDelegateBindingComponent::BindDelegates()
{
}

void UDelegateBindingComponent::BindDelegateFromBP_Implementation()
{
}


void UDelegateBindingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDelegateBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}



