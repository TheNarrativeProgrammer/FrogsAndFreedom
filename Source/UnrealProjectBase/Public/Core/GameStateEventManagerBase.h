// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStateEventManagerBase.generated.h"

class AObjectiveActorBase;
class UEncounterObjective;
//Spawners (machines and vents)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorSpawnerBaseMachineDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorSpawnerBaseMachineDeathSelfRefSignature,
	AActor*, ActorSpawnedRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorSpawnerTankSpawnedSignature,
	int32, SpawnerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterStartActorSpawnerBaseMachineInitialSignature);
//Enemies
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterBaseFrogSpawnedFromPoolSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveOneStartSpawningSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterBaseFrogDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterBaseFrogTankDeathIDSignature,
	int32, ActorUniqueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterBaseFrogTankSpawnedLocationSignature,
	FTransform, InTankLocation, uint32, ActorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterBaseFrogTankMoveLocationSignature,
	FTransform, InTankLocation, uint32, ActorID);
//ForceField
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnForceFieldBaseDeathIDSignature, int32, ActorUniqueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnForceFieldHealthChangeIDHealthSignature,
	int32, ActorUniqueID, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnForceFieldDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEncounterStartForceFieldInitalizedIDHealthSignature,
	int32, ActorUniqueID, float, CurrentHealth, float, MaxHealth);
//GeneratorTimerManager
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGeneratorTimerManagerTimerFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeneratorTimerManagerSecondElaspedSignature,
	float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGeneratorTimerManagerTimerStartSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeneratorTimerManagerTimerStarSendTotalTimeSignature,
	float, TotalTime);
//Generator
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeneratorInteractSendForceFieldIDSignature,
	int32, ActorUniqueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGeneratorInteractSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterStartGeneratorInitializeSignature);
//PooledActorManager
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEndedSignature);
//Player
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeginPlaySendPlayerHealthSignature,
	float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangePlayerSendHealthDataSignature,
	float, CurrentHealth, float, OldHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequestPlayerHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestHealthSendHealthSignature,
	float, CurrentHealth, float, MaxHealth);
//Encounter
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterEndedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorSpawnedSendTransformOfSpawnPointSignature,
	FTransform, SpawnTransform);
//Objectives
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObectivesCompleteSignature);
//Level Reset
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoadResetDataSignature);
//Power cords
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnForceFieldDeactivatedTurnPowerOffSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllForceFieldActivatedTurnPowerOnSignature);
//UI Update
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorStateChangeUpdateUISignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerChangeUpdateUISignature,
	AObjectiveActorBase*, ObjectiveActorBase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorObjectiveCompleteUpdateUISignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorObjectiveCompletionReversedUpdateUISignature,
	AObjectiveActorBase*, ObjectiveActorBase);
//UI Iteract
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIteractEndOverlapUpdateUISignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideTutorialMessageUpdateUISignature);
//Match State - Paused, Unpaused, GameOver
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverClearAllActiveTimersSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGamePausedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameUnPausedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverStopBusSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBusesStopSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBusesUnpausedSignature);
//Dialogue
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStartSignature);

UCLASS()
class UNREALPROJECTBASE_API AGameStateEventManagerBase
	: public AGameState
{
	GENERATED_BODY()

	public:
	//Spawners (machines and vents)
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Spawners")
	FOnEncounterStartActorSpawnerBaseMachineInitialSignature OnEncounterStartActorSpawnerBaseMachineInitialDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Spawners")
	FOnActorSpawnerBaseMachineDeathSignature OnActorSpawnerBaseMachineDeathDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Spawners")
	FOnActorSpawnerBaseMachineDeathSelfRefSignature OnActorSpawnerBaseMachineDeathSelfRefDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Spawners")
	FOnActorSpawnerTankSpawnedSignature OnActorSpawnerTankSpawnedDelegate;
	//Enemies
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Enemies")
	FOnCharacterBaseFrogSpawnedFromPoolSignature OnCharacterBaseFrogSpawnedFromPoolDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Enemies")
	FOnWaveOneStartSpawningSignature OnWaveOneStartSpawningDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Enemies")
	FOnCharacterBaseFrogDeathSignature OnCharacterBaseFrogDeathDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Enemies")
	FOnCharacterBaseFrogTankDeathIDSignature OnCharacterBaseFrogTankDeathIDDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Enemies")
	FOnCharacterBaseFrogTankSpawnedLocationSignature OnCharacterBaseFrogTankSpawnedLocationDelegate;
	//ForceField
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates ForceField")
	FOnForceFieldBaseDeathIDSignature OnForceFieldBaseDeathIDDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates ForceField")
	FOnForceFieldDeathSignature FOnForceFieldDeathDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates ForceField")
	FOnEncounterStartForceFieldInitalizedIDHealthSignature OnEncounterStartForceFieldInitalizedIDHealthDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates ForceField")
	FOnForceFieldHealthChangeIDHealthSignature OnForceFieldHealthChangeIDHealthDelegate;
	//Generator Time Manager
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Generator Timer Manager")
	FOnGeneratorTimerManagerTimerFinishedSignature OnGeneratorTimerManagerTimerFinishedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Generator Timer Manager")
	FOnGeneratorTimerManagerSecondElaspedSignature OnGeneratorTimerManagerSecondElaspedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Generator Timer Manager")
	FOnGeneratorTimerManagerTimerStartSignature OnGeneratorTimerManagerTimerStartDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Generator Timer Manager")
	FOnGeneratorTimerManagerTimerStarSendTotalTimeSignature OnGeneratorTimerManagerTimerStarSendTotalTimeDelegate;
	//Generator
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Computer Generator Activator")
	FOnGeneratorInteractSendForceFieldIDSignature OnGeneratorInteractSendForceFieldIDDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Computer Generator Activator")
	FOnGeneratorInteractSignature OnGeneratorInteractDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Computer Generator Activator")
	FOnEncounterStartGeneratorInitializeSignature OnEncounterStartGeneratorInitializeDelegate;
	//player
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Default")
	FOnBeginPlaySendPlayerHealthSignature OnBeginPlaySendPlayerHealthDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Default")
	FOnHealthChangePlayerSendHealthDataSignature OnHealthChangePlayerSendHealthDataDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Default")
	FOnRequestPlayerHealthSignature OnRequestPlayerHealthDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Default")
	FOnRequestHealthSendHealthSignature OnRequestHealthSendHealthDelegate;
	//Level Reset
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "LevelReset")
	FOnLevelLoadResetDataSignature OnLevelLoadResetDataDelegate;
	//Encounter
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Encouner")
	FOnEncounterStartedSignature OnEncounterStartedDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Encouner")
	FOnEncounterEndedSignature FOnEncounterEndedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates Encouner")
	FOnActorSpawnedSendTransformOfSpawnPointSignature OnActorSpawnedSendTransformOfSpawnPointDelegate;
	//Obejectives
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnAllObectivesCompleteDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnAllObjectiveInfluencingSpawningCompleteDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnAllObjectiveInfluencingDoorOpeningCompleteDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnSingleObectiveCompleteDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnSingleObjectiveInfluencingSpawningCompleteDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnSingleObjectiveInfluencingDoorOpeningCompleteDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Objectives")
	FOnObectivesCompleteSignature OnSingleObectiveCompletionReversedDelegate;
	//Power cords
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate Power Cords")
	FOnForceFieldDeactivatedTurnPowerOffSignature OnForceFieldDeactivatedTurnPowerOffDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate Power Cords")
	FOnAllForceFieldActivatedTurnPowerOnSignature OnAllForceFieldActivatedTurnPowerOnDelegate;
	//UI
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnTimerChangeUpdateUISignature OnTimerChangeUpdateUIDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnActorStateChangeUpdateUISignature OnActorStateChangeUpdateUIDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnActorObjectiveCompleteUpdateUISignature OnActorObjectiveCompleteUpdateUIDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnActorObjectiveCompletionReversedUpdateUISignature OnActorObjectiveCompletionReversedUpdateUIDelegate;
	//UI Iteract
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnIteractEndOverlapUpdateUISignature OnIteractEndOverlapUpdateUIDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnHideTutorialMessageUpdateUISignature OnHideTutorialMessageUpdateUIDelegate;
	//Match State - Paused, Unpaused, GameOver
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnGameOverClearAllActiveTimersSignature OnGameOverClearAllActiveTimersDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnGamePausedSignature OnGamePausedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnGameUnPausedSignature OnGameUnPausedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnBusesUnpausedSignature OnBusesUnpausedDelegate;
	
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnGameOverStopBusSignature OnGameOverStopBusDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnGameOverStopBusSignature OnGameOverStopAllExceuptMusicBusDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate UI")
	FOnBusesStopSignature OnBusesStopDelegate;
	//dialgoue
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate Dialogue")
	FOnDialogueFinishedSignature OnDialogueFinishedDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegate Dialogue")
	FOnDialogueStartSignature OnDialogueStartDelegate;

	UFUNCTION()
	virtual void HandleMatchHasEnded() override;
	
	 UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Events")
	 FOnMatchEndedSignature MatchEnded;

	UFUNCTION(BlueprintCallable)
	void SetHasPlayerWon(bool InHasPlayerWon);

	UFUNCTION(BlueprintCallable)
	bool GetHasPlayerWon();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Events")
	bool _mHasPlayerWon = false;
};
