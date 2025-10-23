// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindingComponentCordMgmr.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindingComponentCordMgmr : public UDelegateBindingComponent
{
	GENERATED_BODY()

	virtual void BindDelegateFromBP_Implementation() override;;
	
	virtual void BindDelegates();
	
};
