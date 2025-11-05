// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindingComponentPlayer.h"
#include "Characters/Player/CharacterBasePlayer.h"


void UDelegateBindingComponentPlayer::BindDelegateFromBP()
{
}

void UDelegateBindingComponentPlayer::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (ACharacterBasePlayer* CharacterBasePlayerOwner = Cast<ACharacterBasePlayer>(GetOwner()))
		{
			GameStateEventManager->OnRequestPlayerHealthDelegate.AddUniqueDynamic(
				CharacterBasePlayerOwner, &ACharacterBasePlayer::OnHealthRequestSendHealth);
			
			GameStateEventManager->OnGamePausedDelegate.AddUniqueDynamic(
				CharacterBasePlayerOwner, &ACharacterBasePlayer::OnGamePausedPauseBuses);
			
			GameStateEventManager->OnGameUnPausedDelegate.AddUniqueDynamic(
				CharacterBasePlayerOwner, &ACharacterBasePlayer::OnGameUnpausedUnPauseBuses);
			
			GameStateEventManager->OnGameOverStopBusDelegate.AddUniqueDynamic(
				CharacterBasePlayerOwner, &ACharacterBasePlayer::OnGameOverStopBuses);
			
			GameStateEventManager->OnGameOverStopAllExceuptMusicBusDelegate.AddUniqueDynamic(
				CharacterBasePlayerOwner,
				&ACharacterBasePlayer::ACharacterBasePlayer::OnGameOverStopAllExecptMusicBuses);

			
		}
	}
}
