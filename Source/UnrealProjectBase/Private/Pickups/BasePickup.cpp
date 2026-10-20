// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/BasePickup.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));

}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePickup::PickUp()
{
	OnPickedUp.Broadcast();
	//destroy this?
}

void ABasePickup::EnteredPickupRange(AActor* ActorInRange)
{
	if (ActorInRange == PlayerCharacter) {
		PickUp();
	}
	
}