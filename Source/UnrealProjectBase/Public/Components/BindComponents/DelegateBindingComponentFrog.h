// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindingComponentFrog.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindingComponentFrog : public UDelegateBindingComponent
{
	GENERATED_BODY()

	virtual void BindDelegateFromBP();
	
	virtual void BindDelegates();
	
};
