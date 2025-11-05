// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.h"
#include "EnemyDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UEnemyDamageable : public UDamageable
{
	GENERATED_BODY()
};

class UNREALPROJECTBASE_API IEnemyDamageable
{
	GENERATED_BODY()
};
