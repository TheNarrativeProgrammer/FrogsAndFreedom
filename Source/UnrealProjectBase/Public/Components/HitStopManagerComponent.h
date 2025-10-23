// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitStopManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class UNREALPROJECTBASE_API UHitStopManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitStopManagerComponent();
	
	UFUNCTION(BlueprintCallable, Blueprintable)
	void StartHitStop(float Duration, float Dilation);
	UFUNCTION(BlueprintCallable, Blueprintable)
	void EndHitStop();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	FTimerHandle EndHitstopTimerHandle;
		
};
