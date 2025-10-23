// Copyright Peasant Pals 2025


#include "Environment/AcidPit/AcidPitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAcidPitBase::AAcidPitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	SetRootComponent(StaticMeshComponent);
	BoxComponent->SetupAttachment(StaticMeshComponent);

}

// Called when the game starts or when spawned
void AAcidPitBase::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAcidPitBase::OnBeginOverlapApplyDamage);
	
}

void AAcidPitBase::OnBeginOverlapApplyDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(OtherActor))
	{
		
		UGameplayStatics::ApplyDamage(CharacterBase, DamageOnEnterAcid, CharacterBase->GetController(), this, TSubclassOf<UDamageType>());
	}
}

// Called every frame
void AAcidPitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

