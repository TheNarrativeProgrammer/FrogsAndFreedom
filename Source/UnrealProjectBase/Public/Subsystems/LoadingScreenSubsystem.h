// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenSubsystem.generated.h"

/**
 * 
 */

UCLASS(Config=Game, DefaultConfig)
class ULoadingScreenSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="LoadingScreen")
	float MinimumLoadingScreenTime = 2.0f;

	UPROPERTY(Config, EditAnywhere, Category="LoadingScreen")
	TSubclassOf<UUserWidget> WidgetClass = nullptr;
	
};
UCLASS()
class UNREALPROJECTBASE_API ULoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void BeginLoadingScreen(const FString& String);
	

	void EndLoadingScreen(UWorld* World);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	bool _mIsGameOpening = true;

	UFUNCTION(BlueprintCallable, Category="LoadingScreen")
	void StopLoadingScreen();

private:
	UPROPERTY(transient)
	UUserWidget* LoadingScreen = nullptr;
	
};
