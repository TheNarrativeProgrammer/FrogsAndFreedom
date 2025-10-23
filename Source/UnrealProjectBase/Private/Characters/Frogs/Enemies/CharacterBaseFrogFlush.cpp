// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Frogs/Enemies/CharacterBaseFrogFlush.h"
#include "Environment/PatrolRoutes/PatrolRouteBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBaseFrogFlush::ACharacterBaseFrogFlush()
{
	SetSpeedToProjectileOrAttack(_mSpeedRunning);
	
}

void ACharacterBaseFrogFlush::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		_mOriginalWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
	}
	
}

void ACharacterBaseFrogFlush::OnPoolActorActivated_Implementation()
{
	Super::OnPoolActorActivated_Implementation();
	IPoolable::OnPoolActorActivated_Implementation();
	DelegateBindingComponentFrog->GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.Broadcast();
}

void ACharacterBaseFrogFlush::AdjustWalkSpeed(float WalkSpeedMultiplier)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed *= WalkSpeedMultiplier;
	}
}



void ACharacterBaseFrogFlush::ResetWalkSpeedToDefault()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = _mOriginalWalkSpeed;
	}
}

void ACharacterBaseFrogFlush::LaunchInDirection_Implementation(float SpeedMultiplier, FVector LocationOfForce)
{
	FVector Direction = GetActorLocation().GetSafeNormal() - LocationOfForce.GetSafeNormal();
	//float ZOffset = 0.05;
	//Direction.Z = ZOffset;
	FVector UpDirection = GetActorUpVector() * SpeedMultiplier;
	FVector LaunchVelocity = (Direction * SpeedMultiplier/100)  + UpDirection ;
	//FVector LaunchVelocity =  UpDirection ;

	LaunchCharacter(LaunchVelocity, true, true);
}

void ACharacterBaseFrogFlush::OnLaunchStartRotation_Implementation()
{
	float Yaw = FMath::RandRange(820, 1580);
	RotatingMovementComponent->RotationRate.Yaw = Yaw;
	RotatingMovementComponent->Activate();

	if (GetWorld()->GetTimerManager().IsTimerActive(CallBackDeactivateRotation) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(CallBackDeactivateRotation, this, &ACharacterBaseFrog::DeactivateRotationComponent, 0.5, false);
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerPaused(CallBackDeactivateRotation) == true)
		{
			GetWorld()->GetTimerManager().UnPauseTimer(CallBackDeactivateRotation);
		}
	}
}

void ACharacterBaseFrogFlush::OnLaunchTriggerFlailAnim_Implementation()
{
	_mIsFlailing = true;
}

APatrolRouteBase* ACharacterBaseFrogFlush::GetPatrolRoute_Implementation()
{
	return PatrolRouteBase;
}

void ACharacterBaseFrogFlush::SetPatrolRoute_Implementation(APatrolRouteBase* InPatrolRoute)
{
	PatrolRouteBase = InPatrolRoute;
}

void ACharacterBaseFrogFlush::IncrumentPatrolRouteIndex_Implementation()
{
	PatrolRouteBase->IncrumentPatrolRoute(_mPatrolIndex, _mDirection);
}

void ACharacterBaseFrogFlush::SetPatrolPointQueryLocation_Implementation(FVector InPatrolPointQueryLocation)
{
	_mPatrolPointQueryLocation = InPatrolPointQueryLocation;
}

FVector ACharacterBaseFrogFlush::GetPatrolPointQueryLocation_Implementation()
{
	return _mPatrolPointQueryLocation;
}

