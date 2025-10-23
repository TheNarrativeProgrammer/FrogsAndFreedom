// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RocketLauncherComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class UNREALPROJECTBASE_API URocketLauncherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URocketLauncherComponent();


protected:
    virtual void BeginPlay() override;

    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);


public:
    // Timer to manage fire rate
    FTimerHandle FireRateTimerHandle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bCanFire = true;

    //FIRE RATE HERE
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate = 1;

    // Call this to try firing the weapon
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartFiring();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopFiring();


    // This will be overridden in Blueprints
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
    void OnFireRocket();

    virtual void OnFireRocket_Implementation();

    void ResetCanFire();
};
