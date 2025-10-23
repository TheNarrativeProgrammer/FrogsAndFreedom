// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "../../Core/GameStateEventManagerBase.h"
#include "GameplayTagContainer.h"
#include "Components/ArrowComponent.h"
#include "ActorSpawnerBase.generated.h"

UENUM()
enum eActorTypeToSpawn : uint8
{
	Flusher,
	Pinner,
	Tank,
	All
};

UCLASS()
class UNREALPROJECTBASE_API AActorSpawnerBase : public AActor,
public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	AActorSpawnerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayTags")
	FGameplayTagContainer GameplayTagContainer;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameStateEventManager")
	// AGameStateEventManagerBase* GameStateEventManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UArrowComponent> FrogSpawnLocation;

	//PROPERTIES - Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Blueprintable, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Blueprintable, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentRoot;

	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
protected:
	virtual void BeginPlay() override;
	
	
	//PROPERTIES - variables
	// UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "SpawnParameters")
	// TEnumAsByte<eActorTypeToSpawn> _mActorSpawnType;

	UPROPERTY(BlueprintReadWrite, Category = "SpawnParameters")
	bool _mCanSpawnActor = true;


	

	

	//FUNCTIONS
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeactivateSpawner();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChangeAppearance();

	UFUNCTION(BlueprintCallable)
	void OnDamangeUpdateState();


public:	
	virtual void Tick(float DeltaTime) override;
	

	UFUNCTION(BlueprintCallable)
	float GetDotProductToPlayer();

	UFUNCTION(BlueprintCallable)
	float GetDistanceToPlayer();

	

	UFUNCTION(BlueprintCallable)
	bool GetCanSpawnActor();

	UFUNCTION(BlueprintCallable)
	void SetCanSpawnActor(bool InCanSpawnActor);

	UFUNCTION(BlueprintCallable)
	FTransform GetTransformSpawneLocation();

};
