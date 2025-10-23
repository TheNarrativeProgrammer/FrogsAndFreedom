// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Components/BindComponents/DelegateBindingComponentFrogTank.h"
#include "Interfaces/Flinchable.h"
#include "CharacterBaseFrogTank.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API ACharacterBaseFrogTank : public ACharacterBaseFrog,
public IFlinchable
{
	GENERATED_BODY()

public:
	ACharacterBaseFrogTank();

	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPoolActorActivated();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindingComponentFrogTank> DelegateBindingComponentFrogTank;

	virtual void OnLaunchTriggerFlinchAnim_Implementation() override;

	UFUNCTION()
	void SendLocation();

private:
	virtual void Death() override;
	
};
