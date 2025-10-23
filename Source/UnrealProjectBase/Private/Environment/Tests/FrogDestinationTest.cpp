// Copyright Peasant Pals 2025


#include "Environment/Tests/FrogDestinationTest.h"

#include "AIController.h"
#include "Engine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Characters/Frogs/Enemies/AI/Controllers/AIControllerMobileFrogBase.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UFrogDestinationTest::RunTest(FEnvQueryInstance& QueryInstance) const
{
	TArray<int32> DistanceLengths;
	TArray<FVector> Positions;
	for (TObjectIterator<ACharacterBaseFrog> Itr; Itr; ++Itr) // Itteration over all Frogs
	{
		if (AAIControllerMobileFrogBase* FrogAIController = Cast<AAIControllerMobileFrogBase>
			((*Itr)->GetController()))
		{
			if (UBlackboardComponent* FrogBlackboardComponent = FrogAIController->GetBlackboardComponent())
			{
				Positions.Add(FrogBlackboardComponent->GetValueAsVector(DestinationLocationName));
			}
		}
	}
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) // Itteration over all points
	{
		DistanceLengths.Empty();
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		for (const FVector& Position : Positions)
		{
			DistanceLengths.Add((Position - ItemLocation).Size());
		}
		float AverageDistance;
		UKismetMathLibrary::AverageOfIntArray(DistanceLengths, AverageDistance);
		It.SetScore(TestPurpose, FilterType, AverageDistance, FloatValueMin.GetValue(), FloatValueMax.GetValue());
	}
}

FText UFrogDestinationTest::GetDescriptionTitle() const
{
	return FText::FromString("Frog Destination Test");
}

FText UFrogDestinationTest::GetDescriptionDetails() const
{
	return FText::FromString("Frog Destination Test Description");
}
