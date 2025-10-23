// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/ControllerCharacterBasePlayer.h"
#include "EnhancedInputSubsystems.h"

void AControllerCharacterBasePlayer::SetupInputComponent()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) 
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				FModifyContextOptions Options;
				Options.bForceImmediately = false;
				Options.bNotifyUserSettings = true;
				Options.bIgnoreAllPressedKeysUntilRelease = true;

				// add the mapping context so we get controls
				Subsystem->AddMappingContext(InputMappingContext, 0, Options);

				UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
			}
		}
	}
}

void AControllerCharacterBasePlayer::BeginPlay()
{
	Super::BeginPlay();

	SetupInputComponent();
	
}