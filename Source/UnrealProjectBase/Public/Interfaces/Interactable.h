// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPROJECTBASE_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseInteractVisual();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractMessageOn(FRotator PlayerRotator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractMessageOff();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract();


};
