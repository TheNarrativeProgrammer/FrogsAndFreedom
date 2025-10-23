// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/BlastRadiusPickup.h"

// Sets default values
ABlastRadiusPickup::ABlastRadiusPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlastRadiusPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlastRadiusPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

