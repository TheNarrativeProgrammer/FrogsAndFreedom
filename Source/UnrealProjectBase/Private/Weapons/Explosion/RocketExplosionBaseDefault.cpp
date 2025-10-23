// Copyright Peasant Pals 2025


#include "Weapons/Explosion/RocketExplosionBaseDefault.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Forceable.h"
#include "Interfaces/PlayerCharacterable.h"


ARocketExplosionBaseDefault::ARocketExplosionBaseDefault()
{
	SphereComponentDamage = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentDamage"));
	SphereComponentForce = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentForce"));
	SphereComponentSense = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentSense"));

	
	SphereComponentSense->SetupAttachment(SceneComponent);
	SphereComponentForce->SetupAttachment(SceneComponent);
	SphereComponentDamage->SetupAttachment(SceneComponent);

	SphereComponentDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponentForce->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponentSense->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponentDamage->PrimaryComponentTick.bCanEverTick = false;
	SphereComponentForce->PrimaryComponentTick.bCanEverTick = false;
	SphereComponentSense->PrimaryComponentTick.bCanEverTick = false;

	SphereComponentDamage->PrimaryComponentTick.bStartWithTickEnabled = false;
	SphereComponentForce->PrimaryComponentTick.bStartWithTickEnabled = false;
	SphereComponentSense->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ARocketExplosionBaseDefault::SetRadiusOrWidithOfCollisionDamage_Implementation()
{
	Super::SetRadiusOrWidithOfCollisionDamage_Implementation();
	_mRadiusOrWidithOfCollisionDamage = SphereComponentDamage->GetUnscaledSphereRadius();
}

void ARocketExplosionBaseDefault::SetRadiusOrWidithOfCollisionForce_Implementation()
{
	Super::SetRadiusOrWidithOfCollisionForce_Implementation();
	_mRadiusOrWidithOfCollisionForce = SphereComponentForce->GetUnscaledSphereRadius();
}

void ARocketExplosionBaseDefault::SetRadiusOrWidithOfCollisionSense_Implementation()
{
	Super::SetRadiusOrWidithOfCollisionSense_Implementation();
	_mRadiusOrWidithOfCollisionSense = SphereComponentSense->GetUnscaledSphereRadius();
}

void ARocketExplosionBaseDefault::SetLocationOfCollision_Implementation()
{
	Super::SetLocationOfCollision_Implementation();
	_mLocationOfCollision = SphereComponentDamage->GetComponentLocation();
}

void ARocketExplosionBaseDefault::IterateOverActorsCollisionDamage_Implementation()
{
	Super::IterateOverActorsCollisionDamage_Implementation();
	bool ReturnValueDamage = false;
	
	//Damage enemies
	TArray<AActor*> ActorsInOutterVolumeToInclude = GetOverlappingActorsInCollision(ReturnValueDamage, _mLocationOfCollision, _mRadiusOrWidithOfCollisionDamage, ObjectTypesDamageOnly);

	for (AActor* ActorInlcude : ActorsInOutterVolumeToInclude)
	{
		CheckIfEnemyBehindCover(ActorInlcude);
	}

	//Launcher Player
	TArray<AActor*> PlayerInDamageCollision = GetOverlappingActorsInCollision(ReturnValueDamage, _mLocationOfCollision, _mRadiusOrWidithOfCollisionDamage, ObjectTypeApplyForceInDamageSphere);

	for (AActor* ForceableActor : PlayerInDamageCollision)
	{
		if (ForceableActor->Implements<UForceable>())
		{
			IForceable::Execute_OnForceApplied(ForceableActor, _mLaunchForce, _mRadiusOrWidithOfCollisionDamage, GetActorLocation());
		}
	}
}

TArray<AActor*> ARocketExplosionBaseDefault::GetOverlappingActorsInCollision_Implementation(bool& InReturnValue,
                                                                                            FVector Location, float RadiusOrWidth, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
{
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsToIgnore;
	InReturnValue = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, RadiusOrWidth, ObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);
	return OutActors;
}












