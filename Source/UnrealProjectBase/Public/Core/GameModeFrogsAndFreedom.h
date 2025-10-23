// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeFrogsAndFreedom.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API AGameModeFrogsAndFreedom : public AGameMode
{
	GENERATED_BODY()

	virtual void ReturnToMainMenuHost() override;

	UFUNCTION(BlueprintCallable, Category = "GameEvents")
	void HandleReturningToMainMenu();
	
};
