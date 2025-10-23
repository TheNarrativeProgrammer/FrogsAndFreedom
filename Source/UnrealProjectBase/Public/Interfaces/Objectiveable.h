// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Objectiveable.generated.h"

UINTERFACE(MinimalAPI)
class UObjectiveable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPROJECTBASE_API IObjectiveable
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsMustCompleteToStopSpawn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsMustCompleteToOpenDoor();
};
