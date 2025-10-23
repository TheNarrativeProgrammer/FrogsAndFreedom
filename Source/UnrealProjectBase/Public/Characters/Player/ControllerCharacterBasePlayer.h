// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControllerCharacterBasePlayer.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API AControllerCharacterBasePlayer : public APlayerController
{
	GENERATED_BODY()
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
};
