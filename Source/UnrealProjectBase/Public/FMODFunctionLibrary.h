// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "FMODBus.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FMODFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UFMODFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Pauses or resumes all FMOD events globally */
	UFUNCTION(BlueprintCallable, Category = "Audio|FMOD")
	static void SetFMODPaused(bool bPaused);


	
};
