// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Environment/ActorSpawner/ActorSpawnerBase.h"
#include "Interfaces/PlayerDamageable.h"
#include "Components/BoxComponent.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "Components/BindComponents/DelegateBindComponentSpawner.h"
#include "Interfaces/ActorSpawnerable.h"
#include "ActorSpawnerBaseFrogMachine.generated.h"

class ACharacterBaseFrogTank;
/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API AActorSpawnerBaseFrogMachine : public AActorSpawnerBase,
public IPlayerDamageable,
public IDelegateBindBroadcastable,
public IActorSpawnerable
{
	GENERATED_BODY()

public:
	AActorSpawnerBaseFrogMachine();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTankDeathCheckIDMatches(int32 ActorID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEncounterStartBroadcastActorInitializationDelegates();

	UFUNCTION(BlueprintCallable)
	FTransform GetTransformSpawnLocationForTank();

	virtual int32 GetActorIDTankAtThisSpawner_Implementation() override;

	virtual int32 GetActorIDOfSpawner_Implementation() override;

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindComponentSpawner> DelegateBindComponentSpawner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnParameters")
	bool _mHasTankFrog = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Blueprintable, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponentDamageArea;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintType, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> TankSpawnLocation;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SpawnParameters")
	bool _mCanTankSpawnHere;

	// UPROPERTY(BlueprintReadOnly, Category = "Frog Tank At this Machine")
	// ACharacterBaseFrogTank* _mCharacterBaseFrogTank;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnParameters")
	int32 _mActorIDTankAtThisSpawner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Frog Tank At this Machine");
	FTransform _mTankLocation;

	

public:

	UFUNCTION()
	void SetActorIDTankAtThisSpawner(uint32 ActorID);

	UFUNCTION()
	void SetTankLocation (FTransform InTankLocation, uint32 InActorID);
	

	UFUNCTION(BlueprintCallable)
	bool GetHasTankFrog();

	UFUNCTION(BlueprintCallable)
	void SetHasTankFrog(bool HasTankFrog);

	UFUNCTION(BlueprintCallable)
	bool GetCanTankSpawnHere();

	// UFUNCTION(BlueprintCallable)
	// ACharacterBaseFrogTank* GetFrogTankAtThisMachine();

	// UFUNCTION()
	// void SetFrogTankAtThisMachine(ACharacterBaseFrogTank* CharacterBaseFrogTank);

	// UFUNCTION(BlueprintCallable)
	// void RegisterListenerToTankDeathDelegate();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnTankDeathRemoveForceField();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnSetTankSetForceFieldLocation();

	virtual bool GetIsShielded_Implementation() override;
	
	
};
