// Copyright Peasant Pals 2025


#include "Subsystems/LevelLoadSubSystem.h"

#include "BehaviorTree/BehaviorTreeTypes.h"

void ULevelLoadSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ULevelLoadSubSystem::OnLevelAddedToWorld);
	
}

bool ULevelLoadSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	 Super::ShouldCreateSubsystem(Outer);
	return true;
}

void ULevelLoadSubSystem::CheckLoadStatus(ULevelStreaming* LevelStreaming)
{
	if (LevelStreaming->IsLevelLoaded() == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(StreamingLevelTimers[LevelStreaming]);
		StreamingLevelTimers.Remove(LevelStreaming);
		OnLevelLoadedDelegate.Broadcast();
	}
	
}

void ULevelLoadSubSystem::StartPollingForLevelLoad(ULevelStreaming* LevelStreaming)
{
	if (!LevelStreaming)
	{
		return;
	}

	//Timer checks periodically if level is loaded
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ULevelLoadSubSystem::CheckLoadStatus, LevelStreaming);

	FTimerHandle& Handle = StreamingLevelTimers.FindOrAdd(LevelStreaming);

	GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, 0.2f, true);
	
}

void ULevelLoadSubSystem::CheckIfLevelHasLoaded(ULevelStreaming* StreamingLevel)
{
	if (!StreamingLevel || !GetWorld())
	{
		return;
	}

	bool IsLoaded = StreamingLevel->IsLevelLoaded();
	bool IsVisible = StreamingLevel->GetLoadedLevel() && StreamingLevel->GetLoadedLevel()->bIsVisible;

	if (IsLoaded == true && IsVisible == true)
	{
		FString LevelName = StreamingLevel->GetWorldAssetPackageName();
		FString OtherLevelName = StreamingLevel->GetName();

		OnLevelLoadedDelegate.Broadcast();

		if (FTimerHandle* Handle = ActivePolls.Find(StreamingLevel))
		{
			GetWorld()->GetTimerManager().ClearTimer(*Handle);
			ActivePolls.Remove(StreamingLevel);
		}
	}
}


void ULevelLoadSubSystem::StartTrackingLevelLoad(const FString& PackageName)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (!StreamingLevel)
		{
			continue;
		}

		if (StreamingLevel->GetWorldAssetPackageName().Contains(PackageName) == true)
		{
			if (!ActivePolls.Contains(StreamingLevel))
			{
				FTimerHandle PollHandle;
				FTimerDelegate PollTimerDelegate;
				PollTimerDelegate.BindUObject(this, &ULevelLoadSubSystem::CheckIfLevelHasLoaded, StreamingLevel);

				World->GetTimerManager().SetTimer(PollHandle, PollTimerDelegate, 0.2f, true);

				ActivePolls.Add(StreamingLevel, PollHandle);
			}
		}
	}
}

void ULevelLoadSubSystem::OnLevelAddedToWorld(ULevel* Level, UWorld* World)
{
	if (!Level || !World)
	{
		return;
	}

	FString LevelName = Level->GetName();
	FString LevelPackageName = Level->GetPackage()->GetName();

	StartTrackingLevelLoad(LevelName);
}
