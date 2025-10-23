// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindingComponentFrogTank.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindingComponentFrogTank : public UDelegateBindingComponent
{
	GENERATED_BODY()

public:
	UDelegateBindingComponentFrogTank();

	virtual void BindDelegateFromBP();
	
	virtual void BindDelegates();
	
};
