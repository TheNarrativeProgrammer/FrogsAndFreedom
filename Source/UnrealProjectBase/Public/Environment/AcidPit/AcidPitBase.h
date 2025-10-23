// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AcidPitBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class UNREALPROJECTBASE_API AAcidPitBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAcidPitBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;


protected:
	// Called when the game starts or when spawned

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Stats")
	float DamageOnEnterAcid;

	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlapApplyDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
