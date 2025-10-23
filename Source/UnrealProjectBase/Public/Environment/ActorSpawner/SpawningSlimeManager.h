// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BindComponents/DelegateBindComponentSlimeManger.h"
#include "SpawningSlimeManager.generated.h"
UCLASS()
class UNREALPROJECTBASE_API ASpawningSlimeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningSlimeManager();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDelegateBindComponentSlimeManger> DelegateBindComponentSlimeManger;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnSlimeAtTranform(FTransform SpawnTransform);
	
	virtual void Tick(float DeltaTime) override;

};
