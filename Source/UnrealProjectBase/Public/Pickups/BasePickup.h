// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BasePickup.generated.h"


class UProjectileMovementComponent;



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpSignature);

UCLASS()
class UNREALPROJECTBASE_API ABasePickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float PickupRange;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float BuffDuration;

	UPROPERTY()
	AActor* PlayerCharacter;

	/*UPROPERTY()
	bool PlayerIsInRange = false;*/
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnPickedUpSignature OnPickedUp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PickUp();

	UFUNCTION(BlueprintCallable)
	void EnteredPickupRange(AActor* ActorInRange);

	//in case you need to interact with pickup to pick it up
	/*UFUNCTION(BlueprintCallable)
	void LeftPickupRange(AActor ActorInRange); */

};
