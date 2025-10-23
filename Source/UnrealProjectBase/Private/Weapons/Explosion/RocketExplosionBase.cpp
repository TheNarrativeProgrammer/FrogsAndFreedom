// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Explosion/RocketExplosionBase.h"

#include "Launchable.h"
#include "Interfaces/Forceable.h"
#include "Interfaces/FrogAIable.h"
#include "Interfaces/PlayerDamageable.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interfaces/Flinchable.h"
#include "Interfaces/PlayerCharacterable.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"

// Sets default values
ARocketExplosionBase::ARocketExplosionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Centre point"));
	SceneComponentTracePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Decal Spawn Location"));

	SceneComponent->SetupAttachment(GetRootComponent());
	SceneComponentTracePoint->SetupAttachment(SceneComponent);

	SceneComponent->PrimaryComponentTick.bCanEverTick = false;
	SceneComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

	SceneComponentTracePoint->PrimaryComponentTick.bCanEverTick = false;
	SceneComponentTracePoint->PrimaryComponentTick.bStartWithTickEnabled = false;

	

}

void ARocketExplosionBase::DestroyAfterImpact_Implementation()
{
	IProjectileInfluencable::DestroyAfterImpact_Implementation();
}

// Called when the game starts or when spawned
void ARocketExplosionBase::BeginPlay()
{
	SpawnParticles();
	SetRadiusOrWidithOfCollisionDamage();
	SetRadiusOrWidithOfCollisionForce();
	SetRadiusOrWidithOfCollisionSense();
	SetLocationOfCollision();
	IterateOverEnemiesCollisionSense();
	IterateOverEnemiesCollisionForce();
	IterateOverActorsCollisionDamage();
	IteratePinnerProjectileAndDestroy();
}

void ARocketExplosionBase::SpawnParticles_Implementation()
{
}


void ARocketExplosionBase::SetRadiusOrWidithOfCollisionForce_Implementation()
{
}

void ARocketExplosionBase::SetRadiusOrWidithOfCollisionDamage_Implementation()
{
}


void ARocketExplosionBase::SetRadiusOrWidithOfCollisionSense_Implementation()
{
}

void ARocketExplosionBase::SetLocationOfCollision_Implementation()
{
}

// Called every frame
void ARocketExplosionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



TArray<AActor*> ARocketExplosionBase::GetOverlappingActorsInCollision_Implementation(
	bool& InReturnValue,FVector Location, float RadiusOrWidth, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
{
	TArray<AActor*> OutActors;
	OutActors.Add(this);
	return OutActors;
}




void ARocketExplosionBase::ApplyDamageToCurrentIteration_Implementation()
{
	if (EnemyActorInCurrentIteration)
	{
		if (EnemyActorInCurrentIteration->Implements<UPlayerDamageable>() == true)
		{
			if (IPlayerDamageable::Execute_GetIsShielded(EnemyActorInCurrentIteration) == false)
			{
				AActor* DamageCauseActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
				AController* InstigatorController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (DamageCauseActor && InstigatorController)
				{
					UGameplayStatics::ApplyDamage(EnemyActorInCurrentIteration, _mDamage, InstigatorController, DamageCauseActor, UDamageType::StaticClass());
				}
			}
				
		}
		if (EnemyActorInCurrentIteration->Implements<UFlinchable>())
		{
			IFlinchable::Execute_OnLaunchTriggerFlinchAnim(EnemyActorInCurrentIteration);
		}
		
	}
}

void ARocketExplosionBase::ApplyHitStopOnGameMode_Implementation(float CountEnemiesDamaged)
{
}

void ARocketExplosionBase::IterateOverEnemiesCollisionSense()
{
	bool ReturnValueInnerVolumeExcludeForce = false;
	TArray<AActor*> ActorsInInnerVolumeToExclude = GetOverlappingActorsInCollision(ReturnValueInnerVolumeExcludeForce, _mLocationOfCollision, _mRadiusOrWidithOfCollisionForce, ObjectTypesForceAndSense);

	bool ReturnValueOutterVolumeIncludeSense = false;
	TArray<AActor*> ActorsInOutterVolumeToInclude = GetOverlappingActorsInCollision(ReturnValueOutterVolumeIncludeSense, _mLocationOfCollision, _mRadiusOrWidithOfCollisionSense, ObjectTypesForceAndSense);

	
	for (AActor* ActorInlcude : ActorsInOutterVolumeToInclude)
	{
		_mIsEnemyInVolumeForce = false;
		for (AActor* ActorExclude : ActorsInInnerVolumeToExclude)
		{
			if (ActorExclude == ActorInlcude)
			{
				_mIsEnemyInVolumeForce = true;
			}
		}
		if (_mIsEnemyInVolumeForce == false)
		{
			AActor* ActorInstigator = Cast<AActor>( UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			UAISense_Damage::ReportDamageEvent(GetWorld(), ActorInlcude, ActorInstigator, 0.0f, ActorInstigator->GetActorLocation(), FVector::Zero(), FName(TEXT("DamageSenceNearByDamage")));
		}
	}
}

void ARocketExplosionBase::IterateOverEnemiesCollisionForce()
{

	bool ReturnValueInnerVolumeExcludeDamage = false;
	TArray<AActor*> ActorsInInnerVolumeToExclude = GetOverlappingActorsInCollision(ReturnValueInnerVolumeExcludeDamage, _mLocationOfCollision, _mRadiusOrWidithOfCollisionDamage, ObjectTypesForceAndSense);

	bool ReturnValueOutterVolumeIncludeForce = false;
	TArray<AActor*> ActorsInOutterVolumeToInclude = GetOverlappingActorsInCollision(ReturnValueOutterVolumeIncludeForce, _mLocationOfCollision, _mRadiusOrWidithOfCollisionForce, ObjectTypesForceAndSense);
	
	for (AActor* ActorInlcude : ActorsInOutterVolumeToInclude)
	{
		_mIsEnemyInVolumeDamge = false;
		for (AActor* ActorExclude : ActorsInInnerVolumeToExclude)
		{
			if (ActorExclude == ActorInlcude)
			{
				_mIsEnemyInVolumeDamge = true;
			}
		}
		if (_mIsEnemyInVolumeDamge == false)
		{
			AActor* ActorInstigator = Cast<AActor>( UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			UAISense_Damage::ReportDamageEvent(GetWorld(), ActorInlcude, ActorInstigator, 0.0f, ActorInstigator->GetActorLocation(), FVector::Zero(), FName(TEXT("DamageSenceForceApplied")));

			if (ActorInlcude->Implements<UFrogAIable>())
			{
				IFrogAIable::Execute_FrozenBehaviour(ActorInlcude);
			}
			if (ActorInlcude->Implements<UPlayerDamageable>())
			{
				if (IPlayerDamageable::Execute_GetIsShielded(ActorInlcude) == false)
				{
					if (ActorInlcude->Implements<ULaunchable>())
					{
						ILaunchable::Execute_OnLaunchTriggerFlailAnim(ActorInlcude);
						ILaunchable::Execute_LaunchInDirection(ActorInlcude, _mForce, _mLocationOfCollision);
						ILaunchable::Execute_OnLaunchStartRotation(ActorInlcude);
					}
				}
			}
		}
	}
	
}

void ARocketExplosionBase::IterateOverActorsCollisionDamage_Implementation()
{
	
}





void ARocketExplosionBase::ApplyDamageIfConditionsMet(AActor* ActorInlcude)
{
	if (_mIsDamageOnlyAppliedWhileAirBorn == true)
	{
		if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			if (PlayerCharacter->GetMovementComponent()->IsFalling() == true)
			{
				EnemyActorInCurrentIteration = ActorInlcude;
				ApplyDamageToCurrentIteration();
			}
		}
	}
	else
	{
		EnemyActorInCurrentIteration = ActorInlcude;
		ApplyDamageToCurrentIteration();
	}
}

void ARocketExplosionBase::IteratePinnerProjectileAndDestroy()
{
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsIgnore;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), _mLocationOfCollision, _mRadiusOrWidithOfCollisionDamage, ObjectTypesProjectile, AActor::StaticClass(), ActorsIgnore, OutActors);

	for (AActor* Actor : OutActors)
	{
		if (Actor->Implements<UProjectileInfluencable>())
		{
			IProjectileInfluencable::Execute_DestroyAfterImpact(Actor);
		}
	}


	OutActors.Empty();
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), _mLocationOfCollision, _mRadiusOrWidithOfCollisionForce, ObjectTypesForceAndSense, AActor::StaticClass(), ActorsIgnore, OutActors);

	for (AActor* Actor : OutActors)
	{
		if (Actor->Implements<UPlayerDamageable>() == true && Actor->Implements<ULaunchable>() == true)
		{
			IPlayerDamageable::Execute_SetIsShielded(Actor, false);
		}
	}

	

	
	
}

void ARocketExplosionBase::CheckIfEnemyBehindCover(AActor* EnemyActor)
{
	EnemyActorInCurrentIteration = EnemyActor;
	if (EnemyActor->Implements<UPlayerDamageable>())
	{
		CheckTraceMiddle();
	}
}

void ARocketExplosionBase::CheckTraceMiddle()
{
	_mForwardVectorScaled = GetActorForwardVector() * _mTraceLength;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation;
	if (EnemyActorInCurrentIteration)
	{
		EndLocation = EnemyActorInCurrentIteration->GetActorLocation();
	}
	else
	{
		EndLocation = StartLocation + _mForwardVectorScaled;

	}
	
	
	FHitResult OutHitResult;

	TArray<AActor*> ActorsToIgore;
	ActorsToIgore.Add(EnemyActorInCurrentIteration);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, TraceTypeQuery, false, ActorsToIgore, EDrawDebugTrace::None, OutHitResult, true);

	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, 10.0f, 0, 10.0f);
	if (OutHitResult.bBlockingHit == true)
	{
		CheckTraceRight();
	}
	else
	{
		ApplyDamageToCurrentIteration();
	}
}

void ARocketExplosionBase::CheckTraceRight()
{
	_mXOffsetLeftAndRight = GetActorRightVector() * _mAmountOffsetToLeftAndRight;

	_mRightTraceStart = GetActorLocation() + _mXOffsetLeftAndRight;
	FVector RightTraceEnd;

	if (EnemyActorInCurrentIteration)
	{
		RightTraceEnd = EnemyActorInCurrentIteration->GetActorLocation();
	}
	else
	{
		RightTraceEnd = _mRightTraceStart + _mForwardVectorScaled;

	}
	

	FHitResult OutHitResult;
	TArray<AActor*> ActorsToIgore;
	ActorsToIgore.Add(EnemyActorInCurrentIteration);

	//DrawDebugLine(GetWorld(), _mRightTraceStart, RightTraceEnd, FColor::Red, true, 10.0f, 0, 10.0f);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), _mRightTraceStart, RightTraceEnd, TraceTypeQuery, false, ActorsToIgore, EDrawDebugTrace::None, OutHitResult, true);
	
	if (OutHitResult.bBlockingHit == true)
	{
		CheckTraceLeft();
	}
	else
	{
		ApplyDamageToCurrentIteration();
	}
	
}

void ARocketExplosionBase::CheckTraceLeft()
{
	_mLeftTraceStart = GetActorLocation() - _mXOffsetLeftAndRight + _mZOffsetLeftAndRight;

	FVector LeftTraceEnd;
	if (EnemyActorInCurrentIteration)
	{
		LeftTraceEnd = EnemyActorInCurrentIteration->GetActorLocation();
	}
	else
	{
		LeftTraceEnd = _mLeftTraceStart + _mForwardVectorScaled;

	}
	

	FHitResult OutHitResult;
	TArray<AActor*> ActorsToIgore;
	ActorsToIgore.Add(EnemyActorInCurrentIteration);

	//DrawDebugLine(GetWorld(), _mLeftTraceStart, LeftTraceEnd, FColor::Green, true, 10.0f, 0, 10.0f);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), _mLeftTraceStart, LeftTraceEnd, TraceTypeQuery, false, ActorsToIgore, EDrawDebugTrace::None, OutHitResult, true);
	
	if (OutHitResult.bBlockingHit == false)
	{
		ApplyDamageToCurrentIteration();
	}
}







