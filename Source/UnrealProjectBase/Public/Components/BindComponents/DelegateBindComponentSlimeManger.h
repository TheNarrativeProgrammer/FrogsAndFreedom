// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindComponentSlimeManger.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindComponentSlimeManger : public UDelegateBindingComponent
{
	GENERATED_BODY()

	virtual void BindDelegates() override;
};
