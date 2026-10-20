// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.h"
#include "HealthPickup.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UNREALPROJECTBASE_API AHealthPickup : public ABasePickup
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	float HealthIncrease;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
