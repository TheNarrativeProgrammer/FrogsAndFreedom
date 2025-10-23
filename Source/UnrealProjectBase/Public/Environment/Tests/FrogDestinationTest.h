// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "FrogDestinationTest.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API UFrogDestinationTest : public UEnvQueryTest
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName DestinationLocationName = FName("DestinationLocation");
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	
};
