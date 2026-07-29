// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthRestoredToDefaultSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthDepletedSelfRefSignature, AActor*, selfRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthDepleatedDecrementCountSignature, int32, NegatedQuantityRemoveFromGamePlay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, OldHealth);


UENUM(BlueprintType)
enum class EStartingHealth : uint8
{
	MaxHealth UMETA(DisplayName = "Max Health"),
	DefaultHealth UMETA(DisplayName = "Default Health"),
	
};


class UDamageType;
class AController;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECTBASE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health Stats")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health Stats")
	float OldHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health Stats")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health Stats")
	float DefaultHealth = MaxHealth;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health Stats")
	EStartingHealth StartingHealth;
	// UPROPERTY(EditDefaultsOnly, Category = "Health Stats")
	// bool bShouldBroadcastIDOnDeath = false;
	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Default")
	FOnHealthChangedSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable,BlueprintReadWrite, Category = "Default")
	FOnHealthDepletedSignature OnHealthDepletedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Default")
	FOnHealthDepletedSelfRefSignature OnHealthDepletedSelfRefDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Default")
	FOnHealthDepleatedDecrementCountSignature OnHealthDepleatedDecrementCountDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Default")
	FOnHealthRestoredToDefaultSignature OnHealthRestoredToDefaultDelegate;
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

public:	
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float DeltaHealth);

	UFUNCTION(BlueprintCallable)
	void ResetHealthToDefault();

	UFUNCTION(BlueprintCallable)
	void ResetHealthToHalfOfDefault();

	UFUNCTION(BlueprintCallable)
	bool IsHealthDepleted();
	
	
};
