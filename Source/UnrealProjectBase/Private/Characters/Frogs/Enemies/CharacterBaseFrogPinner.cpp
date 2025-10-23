// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Frogs/Enemies/CharacterBaseFrogPinner.h"
#include "Environment/PatrolRoutes/PatrolRouteBase.h"

void ACharacterBaseFrogPinner::OnPoolActorActivated_Implementation()
{
	Super::OnPoolActorActivated_Implementation();
	IPoolable::OnPoolActorActivated_Implementation();
	DelegateBindingComponentFrog->GameStateEventManager->OnCharacterBaseFrogSpawnedFromPoolDelegate.Broadcast();
}


ACharacterBaseFrogPinner::ACharacterBaseFrogPinner()
{
	SetSpeedToProjectileOrAttack(_mSpeedOfProjectile);
	
}

void ACharacterBaseFrogPinner::LaunchInDirection_Implementation(float SpeedMultiplier, FVector LocationOfForce)
{
	FVector Direction = GetActorLocation().GetSafeNormal() - LocationOfForce.GetSafeNormal();
	//float ZOffset = 0.05;
	//Direction.Z = ZOffset;
	FVector UpDirection = GetActorUpVector() * SpeedMultiplier;
	//FVector LaunchVelocity = UpDirection;
	FVector LaunchVelocity = (Direction * SpeedMultiplier/100)  + UpDirection ;

	LaunchCharacter(LaunchVelocity, true, true);
	
}

void ACharacterBaseFrogPinner::OnLaunchStartRotation_Implementation()
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

void ACharacterBaseFrogPinner::OnLaunchTriggerFlailAnim_Implementation()
{
	_mIsFlailing = true;
}

APatrolRouteBase* ACharacterBaseFrogPinner::GetPatrolRoute_Implementation()
{
	return PatrolRouteBase;
}

void ACharacterBaseFrogPinner::SetPatrolRoute_Implementation(APatrolRouteBase* InPatrolRoute)
{
	PatrolRouteBase = InPatrolRoute;
}

void ACharacterBaseFrogPinner::IncrumentPatrolRouteIndex_Implementation()
{
	PatrolRouteBase->IncrumentPatrolRoute(_mPatrolIndex, _mDirection);
}

void ACharacterBaseFrogPinner::SetPatrolPointQueryLocation_Implementation(FVector InPatrolPointQueryLocation)
{
	_mPatrolPointQueryLocation = InPatrolPointQueryLocation;
}

FVector ACharacterBaseFrogPinner::GetPatrolPointQueryLocation_Implementation()
{
	return _mPatrolPointQueryLocation;
}

void ACharacterBaseFrogPinner::BeginPlay()
{
	Super::BeginPlay();
	// if (!AttackTokenRequesterComponent)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("AttackTokenRequesterComponent in CharacterBaseFrogPinner is null on BeginPlay"));
	// }
	// else if (AttackTokenRequesterComponent->_maRequestedTokenIndexes.IsEmpty() == true)
	// {
	// 	AttackTokenRequesterComponent->_maRequestedTokenIndexes.Add(3);
	// 	AttackTokenRequesterComponent->_maRequestedTokenIndexes.Add(4);
	// 	AttackTokenRequesterComponent->_maRequestedTokenIndexes.Add(5);
	// }
}


