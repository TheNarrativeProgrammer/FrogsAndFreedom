// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Weapons/PlayerWeapon/RocketLauncherComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "PowerupFireRateable.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "Components/BindComponents/DelegateBindingComponentPlayer.h"
#include "../../Interfaces/Interactable.h"
#include "Interfaces/Token/AttackTokenGiverable.h"

#include "../../Interfaces/EnemyDamageable.h"
#include "Interfaces/PlayerCharacterable.h"
#include "Interfaces/PowerupBlastRadiusable.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "CharacterBasePlayer.generated.h"


/**
 * 
 */

class UPlayerCharacterable;
class UInputMappingContext;
class UInputAction;
class UWeaponComponent;
class UBoxComponent;
class USceneComponent;
class UAttackTokenGiverComponent;

UCLASS()
class UNREALPROJECTBASE_API ACharacterBasePlayer : public ACharacterBase,
public IEnemyDamageable,
public IDelegateBindBroadcastable,
public IAttackTokenGiverable,
public IPlayerCharacterable,
public IPowerupBlastRadiusable,
public IPowerupFireRateable

{
	GENERATED_BODY()

public:
	ACharacterBasePlayer();

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// TArray<FVector> GetEnemyAttackLocations();

	UFUNCTION()
	void PopulateEnemyAttackLocationsArray(USceneComponent* EnemyAttackLocation);


	//IAttackTokenGiverable Function implementation
	virtual void HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint) override;

	virtual bool HandleResevereTokenRequest_Implementation(const FRequesterGeneralTokenData& RequesterTokenData, const TArray<int32>& RequestedIndexes, FRequesterReservationData& ReservationData) override;


	virtual FVector HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint) override;

	//PowerupBlastRadiusable functions
	virtual bool GetIsBlastRadiusPowerupEnabled_Implementation() override;

	virtual void SetIsBlastRadiusPowerupEnabled_Implementation(bool InIsBlastRadiusPowerUpEnabled) override;

	virtual void OnOverlapCallBlastRadiusPickupTimer_Implementation() override;

	//PowerupFireRate functions
	virtual void OnOverlapROFPickupTimer_Implementation() override;


	//function when get is pause
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGamePausedPauseBuses();

	UFUNCTION(BlueprintCallable,  BlueprintNativeEvent)
	void OnGameUnpausedUnPauseBuses();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameOverStopBuses();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameOverStopAllExecptMusicBuses();

	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadWrite, Category = "Paused Status")
	bool _mIsPaused = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	UFUNCTION(BlueprintCallable)
	void OnHealthChanageSnedHealthInfo(float CurrentHealth, float OldHealth);

	//PROPERIES - enemy attack location
	UPROPERTY()
	TArray<USceneComponent*> _maEnemyAttackLocations;

	//PROPERTIES - mapping context
    /**DEFAULTMAPPINGCONTEXT - Input Mapping Context for standard inputs. Input actions are linked to mapping both
     *in C++ and with engine.*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* DefaultMappingContext;


	//PROPERTIES - Input actions
    /**LOOKACTION --> input ACTION for looking. Related to mouse movement*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* LookAction;

    /**FIREACTION --> Input Action for firing. Related to left mouse*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* FireAction;
    // * is a raw point that doesnt send to GC

     /**JUMPACTION --> Jump Input Action. Related to space bar, but this feature might be removed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;

    /**MOVEACTION --> Move Input Action. Related to WASD */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;

	//NOTE: other input actions, such as E for interact, are done through the mapping within the engine.

 
    //PROPERTIES - components
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    URocketLauncherComponent* RocketLauncher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponentInteraction;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationLeft;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationMid;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationRight;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// TObjectPtr<USceneComponent> SceneComponentPinnerAimLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UDelegateBindingComponentPlayer> DelegateBindingComponentPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttackTokenGiverComponent> AttackTokenGiverableComponent;

	//PROPERITIES - overlap events persistent data
	/*OVERLAPACTORINTERACTABLE --> set when player overlaps with object that implemenents IInteractable interface
	 *set to null when player ends the overlap*/
	UPROPERTY(BlueprintReadWrite, Category = "Overlap data")
	AActor* OverlapActorInteractable;

	//Property tracking blast radius powerUp
	UPROPERTY(BlueprintReadWrite, Category = "PowerUp Tracking")
	bool _mIsBlastRadiusPowerUpEnabled = false;

	//FUNCTIONS - movements
    /**MOVE --> Called for movement input */
    UFUNCTION()
    void Move(const FInputActionValue& Value);

    /** LOOK --> Called for looking input */
    UFUNCTION()
    void Look(const FInputActionValue& Value);

	/**SETUPPLAYERINPUTCOMPONENT --> Enhanced Input setup */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Called when fire input is pressed */
    //UFUNCTION()
    //void HandleFireStarted(const FInputActionInstance& Instance);

    /** Called when fire input is released */
    //UFUNCTION()
    //void HandleFireStopped(const FInputActionInstance& Instance);

	//FUNCTIONS - overlap
	/*ONBEGINOVERLAPFORINTERACTABLEOGGLEMESSAGEON --> check if overlapped actor implements IInteracble interface. If is does
	 *then cast OtherActor to interface and call InteractMessageOn function, passing in the players rotation. This turns
	 *on the interact message and sets it's rotation to the player's so the message points toward them.*/
	UFUNCTION()
	void OnBeginOverlapForInteractableToggleMessageOn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/*ONENDOVERLAPFORINTERACTABLETOGGLEMESSAGEOFF --> check if overlapped actor implements IInteracble interface. If is does
	 *then cast OtherActor to interface and call InteractMessageOff function. This sets interact message visiblity to false*/
	UFUNCTION()
	void OnEndOverlapForInteractableToggleMessageOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//FUNCTIONS - Getter for EQS tasks
	/*GETENEMYATTACKLOCATION --> Returns the attack location for enemies. EQS needs a position that isn't the current player position
	 *If enemy behaviour tree uses player location to MoveTo, then it never reaches the location if the player stands still.
	 *This gives the enemy a point in front of the player to move to that isn't blocked by the player.*/
	// UFUNCTION(BlueprintCallable)
	// FVector GetEnemeyAttackLocation();

	UFUNCTION(BlueprintCallable)
	void OnHealthChangeSendHealth(float NewHealth, float OldHealth);

public:
	UFUNCTION(BlueprintCallable)
	void OnHealthRequestSendHealth();

	
	
	
    
};
