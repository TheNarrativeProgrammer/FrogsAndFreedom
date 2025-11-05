// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Characters/Player/CharacterBasePlayer.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OldHealth = CurrentHealth;
	OnHealthChangedDelegate.Broadcast(
		OldHealth, CurrentHealth);
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddUniqueDynamic(
			this, &UHealthComponent::TakeDamage);
	}
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const 
{
	return MaxHealth;
}

void UHealthComponent::UpdateHealth(float DeltaHealth)
{
	OldHealth = CurrentHealth;
	CurrentHealth += DeltaHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	
	OnHealthChangedDelegate.Broadcast(
		CurrentHealth, MaxHealth);
	
	
	
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		OnHealthDepletedDelegate.Broadcast();
		OnHealthDepletedSelfRefDelegate.Broadcast(GetOwner());
	}
}

void UHealthComponent::ResetHealthToDefault()
{
	CurrentHealth = MaxHealth;
	OnHealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
	
	
}

void UHealthComponent::ResetHealthToHalfOfDefault()
{
	CurrentHealth = (MaxHealth / 2);
	OnHealthChangedDelegate.Broadcast(CurrentHealth, MaxHealth);
	
}

bool UHealthComponent::IsHealthDepleted()
{
	if (CurrentHealth <= 0.0f)
	{
		return true;
	}
	return false;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor,
	float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser) 
{
	OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	OnHealthChangedDelegate.Broadcast(CurrentHealth, OldHealth);
	
	if (IsHealthDepleted())
	{
		OnHealthDepletedDelegate.Broadcast();
	}
}


