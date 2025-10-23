// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelLoadSubSystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoadedSignature);

UCLASS()
class UNREALPROJECTBASE_API ULevelLoadSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void CheckLoadStatus(ULevelStreaming* LevelStreaming);
	
	void StartPollingForLevelLoad(ULevelStreaming* LevelStreaming);
	
	void CheckIfLevelHasLoaded(ULevelStreaming* StreamingLevel);

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Delegates")
	FOnLevelLoadedSignature OnLevelLoadedDelegate;

	TMap<ULevelStreaming*, FTimerHandle> StreamingLevelTimers;

	TMap<ULevelStreaming*, FTimerHandle> ActivePolls;

	void StartTrackingLevelLoad(const FString& PackageName);

	void OnLevelAddedToWorld(ULevel* Level, UWorld* World);
	
};
