// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindComponentComputer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindComponentComputer : public UDelegateBindingComponent
{
	GENERATED_BODY()

public:
	virtual void BindDelegateFromBP();
	
	virtual void BindDelegates();
	
};
