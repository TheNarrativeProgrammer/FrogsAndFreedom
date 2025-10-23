// Copyright Peasant Pals 2025


#include "Components/DelegateBindingComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDelegateBindingComponent::UDelegateBindingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UDelegateBindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDelegateBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UDelegateBindingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>( UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
	}
	BindDelegates();
	BindDelegateFromBP();
}

void UDelegateBindingComponent::BindDelegates()
{
	UE_LOG(LogTemp, Display, TEXT("UDelegateBindingComponent::BindDelegates"));
	
	// if (AActor* Actor = GetOwner())
	// {
	// 	if (Actor->GetClass()->ImplementsInterface(UDelegateBindBroadcastable::StaticClass()))
	// 	{
	// 		IDelegateBindBroadcastable::Execute_OnDelegateComponentInitalizedBindDelegates(Actor);
	// 	}
	// }

	
}

void UDelegateBindingComponent::BindDelegateFromBP_Implementation()
{
}



