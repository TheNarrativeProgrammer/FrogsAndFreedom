// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Core/GameStateEventManagerBase.h"
#include "DelegateBindingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class UNREALPROJECTBASE_API UDelegateBindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDelegateBindingComponent();

	UPROPERTY(BlueprintReadWrite, Blueprintable)
	AGameStateEventManagerBase* GameStateEventManager;

	// UFUNCTION(BlueprintNativeEvent)
	// void BindDelegateFromBP();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializeComponent() override;

	UFUNCTION()
	virtual void BindDelegates();

	UFUNCTION(BlueprintNativeEvent)
	void BindDelegateFromBP();

public:	

		
};
