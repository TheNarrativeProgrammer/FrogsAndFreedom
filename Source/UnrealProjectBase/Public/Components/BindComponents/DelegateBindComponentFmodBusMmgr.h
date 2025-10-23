// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindComponentFmodBusMmgr.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindComponentFmodBusMmgr : public UDelegateBindingComponent
{
	GENERATED_BODY()

	virtual void BindDelegates() override;

	virtual void BindDelegateFromBP_Implementation() override;
	
};
