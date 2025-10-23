// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/BindComponents/DelegateBindComponentFmodBusMmgr.h"
#include "GameFramework/Actor.h"
#include "FmodBusManager.generated.h"

UCLASS()
class UNREALPROJECTBASE_API AFmodBusManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFmodBusManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentFmodBusMmgr> DelegateBindComponentFmodBusMmgr;

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGamePausePauseFmodBuses();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameUnpauseUnpauseFmodBuses();


};
