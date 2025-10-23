// Copyright Peasant Pals 2025


#include "Subsystems/LoadingScreenSubsystem.h"

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

void ULoadingScreenSubsystem::BeginLoadingScreen(const FString& String)
{
	const ULoadingScreenSetting* Settings = GetDefault<ULoadingScreenSetting>();
	FLoadingScreenAttributes LoadingScreenAttributes;
	
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;
	
	
	if (GetMoviePlayer()->IsMovieCurrentlyPlaying() == false && _mIsGameOpening == false)
	{
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = Settings->MinimumLoadingScreenTime;
		LoadingScreenAttributes.bAllowEngineTick = true;
		LoadingScreenAttributes.bWaitForManualStop = true;
	
		LoadingScreen = CreateWidget(GetGameInstance(),Settings->WidgetClass);
		LoadingScreenAttributes.WidgetLoadingScreen = LoadingScreen->TakeWidget();
	
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
	}
	
}



void ULoadingScreenSubsystem::EndLoadingScreen(UWorld* World)
{
	if (_mIsGameOpening == false)
	{
		GetMoviePlayer()->StopMovie();
		LoadingScreen = nullptr;
	}
	if (GetMoviePlayer()->IsStartupMoviePlaying() == true)
	{
		_mIsGameOpening = false;
	}
	
}

bool ULoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	Super::ShouldCreateSubsystem(Outer);
	return true;
}



void ULoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingScreenSubsystem::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenSubsystem::EndLoadingScreen);
}

void ULoadingScreenSubsystem::StopLoadingScreen()
{

	if (GetMoviePlayer()->IsLoadingFinished() == true)
	{
		if (GetMoviePlayer()->IsStartupMoviePlaying() == false)
		{
			GetMoviePlayer()->StopMovie();
			LoadingScreen = nullptr;
		}
		
	}
}


