// Copyright Peasant Pals 2025


#include "FMODFunctionLibrary.h"
#include "FMODStudioModule.h"
#include "FMODBus.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "FMODBlueprintStatics.h"


void UFMODFunctionLibrary::SetFMODPaused(bool bPaused)
{
	
	//Get FMOD Studio System (correct call)
	if (IFMODStudioModule::IsAvailable())
	{
		FMOD::Studio::System* StudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
		
		if (StudioSystem)
		{
			// et Core FMOD system
			FMOD::System* CoreSystem = nullptr;
			FMOD_RESULT Result = StudioSystem->getCoreSystem(&CoreSystem);

			if (Result != FMOD_OK || !CoreSystem)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get FMOD Core System: %d"), Result);
				return;
			}

			//Get Master Channel Group
			FMOD::ChannelGroup* MasterGroup = nullptr;
			Result = CoreSystem->getMasterChannelGroup(&MasterGroup);

			if (Result != FMOD_OK || !MasterGroup)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get Master Channel Group: %d"), Result);
				return;
			}

			//Pause/unpause globally
			Result = MasterGroup->setPaused(bPaused);

			if (Result != FMOD_OK)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to setPaused: %d"), Result);
			}
		}
		
	}

	
}


