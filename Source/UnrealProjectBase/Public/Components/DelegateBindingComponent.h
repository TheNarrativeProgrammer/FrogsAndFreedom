// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Core/GameStateEventManagerBase.h"
#include "DelegateBindingComponent.generated.h"


UCLASS( ClassGroup=(Custom),
	meta=(BlueprintSpawnableComponent),
	Blueprintable, BlueprintType )
class UNREALPROJECTBASE_API UDelegateBindingComponent
	: public UActorComponent
{
	GENERATED_BODY()
public:	
	UDelegateBindingComponent();

	UPROPERTY(BlueprintReadWrite, Blueprintable)
	AGameStateEventManagerBase* GameStateEventManager;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const
		EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializeComponent() override;

	UFUNCTION()
	virtual void BindDelegates();

	UFUNCTION(BlueprintNativeEvent)
	void BindDelegateFromBP();
	
};
