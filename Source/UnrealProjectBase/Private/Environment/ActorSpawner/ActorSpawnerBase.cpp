// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/ActorSpawnerBase.h"

#include "DelayAction.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActorSpawnerBase::AActorSpawnerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	//add components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SceneComponentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentRoot"));
	FrogSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("FrogSpawnLocation"));
	
	//set attachments
	SetRootComponent(SceneComponentRoot);
	StaticMeshComponent->SetupAttachment(SceneComponentRoot);
	FrogSpawnLocation->SetupAttachment(SceneComponentRoot);
	
	//set tags
	FrogSpawnLocation->ComponentTags.Add(FName("SpawnLocation"));
	

	//FrogSpawnLocation->bEditableWhenInherited = false;
	FrogSpawnLocation->SetMobility(EComponentMobility::Type::Movable);

}


void AActorSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	//bind delegates
}

void AActorSpawnerBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}


void AActorSpawnerBase::DeactivateSpawner_Implementation()
{
}

void AActorSpawnerBase::ChangeAppearance_Implementation()
{
}

void AActorSpawnerBase::OnDamangeUpdateState()
{
	DeactivateSpawner();
	ChangeAppearance();
}



float AActorSpawnerBase::GetDotProductToPlayer()
{
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerActor)
	{
		return -1;
	}
	//direction from player to this object
	FVector DirectionPlayerToThis = GetActorLocation().GetSafeNormal() - PlayerActor->GetActorLocation().GetSafeNormal();
	
	//players forward direction
	FVector DirectionPlayerForward = PlayerActor->GetActorForwardVector();
	
	//dot production
	float DotProductPlayerToThis = FVector::DotProduct(DirectionPlayerForward, DirectionPlayerToThis);
	return DotProductPlayerToThis;
}

float AActorSpawnerBase::GetDistanceToPlayer()
{
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerActor)
	{
		return 10000;
	}
	FVector VectorThisToPlayer = PlayerActor->GetActorLocation() - GetActorLocation();
	float DistanceThisToPlayer = VectorThisToPlayer.Size();
	return DistanceThisToPlayer;
}



bool AActorSpawnerBase::GetCanSpawnActor()
{
	return _mCanSpawnActor;
}

void AActorSpawnerBase::SetCanSpawnActor(bool InCanSpawnActor)
{
	_mCanSpawnActor = InCanSpawnActor;
}


FTransform AActorSpawnerBase::GetTransformSpawneLocation()
{
	return FrogSpawnLocation->GetComponentTransform();
	//return FTransform::Identity;
}

// Called every frame
void AActorSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

