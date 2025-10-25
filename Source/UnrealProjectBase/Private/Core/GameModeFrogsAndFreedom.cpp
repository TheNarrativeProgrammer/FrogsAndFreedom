// Copyright Peasant Pals 2025


#include "Core/GameModeFrogsAndFreedom.h"

#include "Core/GameStateEventManagerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/DialogueSubsystem.h"

void AGameModeFrogsAndFreedom::ReturnToMainMenuHost()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	if (AGameStateEventManagerBase* GameStateEventManagerBase =
		Cast<AGameStateEventManagerBase>(World->GetGameState()))
	{
		GameStateEventManagerBase->OnGameOverClearAllActiveTimersDelegate.Broadcast();
	}
		
	if (UDialogueSubsystem* DialogueSubsystem =
		GetGameInstance()->GetSubsystem<UDialogueSubsystem>())
	{
		DialogueSubsystem->OnGameOverClearTimer();
	}
	
	FInputModeUIOnly InputMode;
	World->GetFirstPlayerController()->SetInputMode(InputMode);
	World->GetFirstPlayerController()->bShowMouseCursor = true;
	GEngine->ForceGarbageCollection(true);
	Super::ReturnToMainMenuHost();
}

void AGameModeFrogsAndFreedom::HandleReturningToMainMenu()
{
	ReturnToMainMenuHost();
}
