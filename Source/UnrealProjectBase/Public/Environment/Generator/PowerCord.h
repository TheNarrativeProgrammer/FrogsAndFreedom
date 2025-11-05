// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerCord.generated.h"

UCLASS()
class UNREALPROJECTBASE_API APowerCord : public AActor
{
	GENERATED_BODY()
public:	
	APowerCord();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PowerCord")
	void TurnPowerCordsOn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PowerCord")
	void TurnPowerCordsOff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
