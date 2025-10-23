// Copyright Peasant Pals 2025


#include "Components/BindComponents/DelegateBindComponentLevelDoor.h"
#include "Environment/Doors/LevelDoorBase.h"

void UDelegateBindComponentLevelDoor::BindDelegateFromBP()
{
	Super::BindDelegateFromBP();
}

void UDelegateBindComponentLevelDoor::BindDelegates()
{
	Super::BindDelegates();
	if (GameStateEventManager)
	{
		if (ALevelDoorBase* LevelDoorOwner = Cast<ALevelDoorBase>(GetOwner()))
		{
			GameStateEventManager->OnAllObjectiveInfluencingDoorOpeningCompleteDelegate.AddUniqueDynamic(
				LevelDoorOwner, &ALevelDoorBase::OpenDoor);
		}
	}
	
	
}
