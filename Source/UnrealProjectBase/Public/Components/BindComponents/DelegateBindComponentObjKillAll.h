// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/DelegateBindingComponent.h"
#include "DelegateBindComponentObjKillAll.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UDelegateBindComponentObjKillAll : public UDelegateBindingComponent
{
	GENERATED_BODY()

	virtual void BindDelegateFromBP_Implementation() override;

	virtual void BindDelegates() override;
	
};
