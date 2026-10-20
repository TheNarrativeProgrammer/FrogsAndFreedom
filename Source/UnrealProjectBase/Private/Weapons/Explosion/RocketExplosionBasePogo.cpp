// Copyright Peasant Pals 2025


#include "Weapons/Explosion/RocketExplosionBasePogo.h"

#include "DestructibleInterface.h"
#include "Components/BoxComponent.h"
#include "Interfaces/BPI_Destructible.h"
#include "Interfaces/Forceable.h"
#include "Interfaces/PlayerDamageable.h"
#include "Kismet/KismetSystemLibrary.h"

ARocketExplosionBasePogo::ARocketExplosionBasePogo()
{
	BoxComponentDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentDamage"));
	BoxComponentForce = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentForce"));
	BoxComponentSense = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentSense"));

	BoxComponentDamage->SetupAttachment(SceneComponent);
	BoxComponentForce->SetupAttachment(SceneComponent);
	BoxComponentSense->SetupAttachment(SceneComponent);

	BoxComponentDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponentForce->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponentSense->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxComponentDamage->PrimaryComponentTick.bCanEverTick = false;
	BoxComponentForce->PrimaryComponentTick.bCanEverTick = false;
	BoxComponentSense->PrimaryComponentTick.bCanEverTick = false;

	BoxComponentDamage->PrimaryComponentTick.bStartWithTickEnabled = false;
	BoxComponentForce->PrimaryComponentTick.bStartWithTickEnabled = false;
	BoxComponentSense->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ARocketExplosionBasePogo::SetRadiusOrWidithOfCollisionDamage_Implementation()
{
	Super::SetRadiusOrWidithOfCollisionDamage_Implementation();
	_mRadiusOrWidithOfCollisionDamage = BoxComponentDamage->GetUnscaledBoxExtent().X;
}

void ARocketExplosionBasePogo::SetRadiusOrWidithOfCollisionForce_Implementation()
{
	Super::SetRadiusOrWidithOfCollisionForce_Implementation();
	_mRadiusOrWidithOfCollisionForce = BoxComponentForce->GetUnscaledBoxExtent().X;
}

void ARocketExplosionBasePogo::SetRadiusOrWidithOfCollisionSense_Implementation()
{
	Super::SetRadiusOrWidithOfCollisionSense_Implementation();
	_mRadiusOrWidithOfCollisionSense = BoxComponentSense->GetUnscaledBoxExtent().X;
}

void ARocketExplosionBasePogo::SetLocationOfCollision_Implementation()
{
	Super::SetLocationOfCollision_Implementation();
	_mLocationOfCollision = BoxComponentDamage->GetComponentLocation();
}

void ARocketExplosionBasePogo::IterateOverActorsCollisionDamage_Implementation()
{
	Super::IterateOverActorsCollisionDamage_Implementation();
	bool ReturnValueDamage = false;
	TArray<AActor*> ActorsInOutterVolumeToInclude = GetOverlappingActorsInCollision(ReturnValueDamage, _mLocationOfCollision, _mRadiusOrWidithOfCollisionDamage, ObjectTypesDamageOnly);

	for (AActor* ActorInlcude : ActorsInOutterVolumeToInclude)
	{
		ApplyDamageIfConditionsMet(ActorInlcude);
	}

	
	//Force only destructibles. Launching the player is handled by different code.
	TArray<AActor*> DestructiblesInDamageCollision = GetOverlappingActorsInCollision(ReturnValueDamage, _mLocationOfCollision, _mRadiusOrWidithOfCollisionDamage, ObjectTypeApplyForceInDamageSphere);
	for (AActor* ForceableActor : DestructiblesInDamageCollision)
	{
		if (ForceableActor->Implements<UForceable>() && ForceableActor->Implements<UBPI_Destructible>())
		{
			if (ForceableActor->Implements<UPlayerDamageable>() == true)
			{
				if (IPlayerDamageable::Execute_GetIsShielded(ForceableActor) == false)
				{
					IForceable::Execute_OnForceApplied(ForceableActor, _mLaunchForce, _mRadiusOrWidithOfCollisionDamage, GetActorLocation());
				}
			}
			else
			{
				IForceable::Execute_OnForceApplied(ForceableActor, _mLaunchForce, _mRadiusOrWidithOfCollisionDamage, GetActorLocation());
			}
			
		}
	}
	
}

TArray<AActor*> ARocketExplosionBasePogo::GetOverlappingActorsInCollision_Implementation(bool& InReturnValue,
                                                                                         FVector Location, float RadiusOrWidth, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
{
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsToIgnore;
	InReturnValue = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Location, FVector(RadiusOrWidth, RadiusOrWidth, _mBoxHeightZaxis), ObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);
	//InReturnValue = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, RadiusOrWidth, ObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);

	//DrawDebugBox(GetWorld(), Location, FVector(RadiusOrWidth, RadiusOrWidth, _mBoxHeightZaxis), FColor::Red, false, 4.0f);
	return OutActors;
}

void ARocketExplosionBasePogo::BeginPlay()
{
	_mBoxHeightZaxis = BoxComponentDamage->GetUnscaledBoxExtent().Z;
	_mIsEffectedByCover = false;
	_mIsDamageOnlyAppliedWhileAirBorn = false;
	Super::BeginPlay();
}
