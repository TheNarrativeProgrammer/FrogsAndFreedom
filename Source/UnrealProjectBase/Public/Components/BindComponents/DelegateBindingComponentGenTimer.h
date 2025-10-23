// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindingComponentGenTimer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindingComponentGenTimer : public UDelegateBindingComponent
{
	GENERATED_BODY()

public:
	virtual void BindDelegateFromBP_Implementation() override;;
	
	virtual void BindDelegates();
	
};
