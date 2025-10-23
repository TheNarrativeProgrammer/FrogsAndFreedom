// Copyright Peasant Pals 2025


#include "Events/DialogueTrigger.h"

#include "Subsystems/DialogueSubsystem.h"

// Sets default values
ADialogueTrigger::ADialogueTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentDialogueTrigger"));

}

// Called when the game starts or when spawned
void ADialogueTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADialogueTrigger::OnOverlapTriggerDialague);
	
}

void ADialogueTrigger::OnOverlapTriggerDialague_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ADialogueTrigger::OnOverlapTriggerDialague);
	
	if (UDialogueSubsystem* DialogueSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueSubsystem>())
	{
		DialogueSubsystem->AddDialogueTextDataToQue(GameplayTag.GetTagName());
		DialogueSubsystem->TriggerDialogueIfNonPlaying();
	}
	
}




