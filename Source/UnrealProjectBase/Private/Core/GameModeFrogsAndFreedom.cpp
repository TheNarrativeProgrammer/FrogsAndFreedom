// Copyright Peasant Pals 2025


#include "Core/GameModeFrogsAndFreedom.h"

#include "Core/GameStateEventManagerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/DialogueSubsystem.h"

void AGameModeFrogsAndFreedom::ReturnToMainMenuHost()
{
	
	
	if (AGameStateEventManagerBase* GameStateEventManagerBase = Cast<AGameStateEventManagerBase>(GetWorld()->GetGameState()))
	{
		GameStateEventManagerBase->OnGameOverClearAllActiveTimersDelegate.Broadcast();
		UDialogueSubsystem* DialogueSubsystem = GetGameInstance()->GetSubsystem<UDialogueSubsystem>();
		if (DialogueSubsystem)
		{
			DialogueSubsystem->OnGameOverClearTimer();
		}
	}
	
	FInputModeUIOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	GEngine->ForceGarbageCollection(true);

	//GetGameInstance()->ReturnToMainMenu();
	Super::ReturnToMainMenuHost();
}

void AGameModeFrogsAndFreedom::HandleReturningToMainMenu()
{
	ReturnToMainMenuHost();
}
