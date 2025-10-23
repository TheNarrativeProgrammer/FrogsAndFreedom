// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BindComponents/DelegateBindingComponentCordMgmr.h"
#include "PowerCordManager.generated.h"

class APowerCord;
UCLASS()
class UNREALPROJECTBASE_API APowerCordManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerCordManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindingComponentCordMgmr> DelegateBindingComponentCordMgmr;
	
	
	UFUNCTION()
	void TurPowerCordsOn();
	
	UFUNCTION()
	void TurnPowerCordsOff();

protected:
	

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="PowerCords")
	TSet<TObjectPtr<APowerCord>> _msPowerCordsInLevel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
