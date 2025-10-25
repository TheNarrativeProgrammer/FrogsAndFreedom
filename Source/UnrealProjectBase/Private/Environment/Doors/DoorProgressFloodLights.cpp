// Copyright Peasant Pals 2025


#include "Environment/Doors/DoorProgressFloodLights.h"

// Sets default values
ADoorProgressFloodLights::ADoorProgressFloodLights()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADoorProgressFloodLights::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoorProgressFloodLights::AddLightToLightDatas(UStaticMeshComponent* StaticMeshComponentLight)
{
	FLightData LightData;
	LightData.StaticMeshComponentLight = StaticMeshComponentLight;
	_aLightDatas.AddUnique(LightData);
}

void ADoorProgressFloodLights::AssignTimeToTurnOnLight(float TimeLengthObjectiveGeneratorDefence)
{
	if (_aLightDatas.IsEmpty()) return;
	
	int32 arrayLength = _aLightDatas.Num();
	int32 index = arrayLength -1;
	for (auto& LightData : _aLightDatas)
	{
		LightData.TimeToTurnOnLight = (TimeLengthObjectiveGeneratorDefence / arrayLength) * index;
		index--;
	}
}

void ADoorProgressFloodLights::CheckIfLightCanTurnOn(float RemainingTime)
{
	if (RemainingTime > _aLightDatas[indexToCheck].TimeToTurnOnLight ||
		_aLightDatas[indexToCheck].IsTurnedOn == true)
	{
		return;
	}
	TurnOnLight(_aLightDatas[indexToCheck].StaticMeshComponentLight, indexToCheck);
}

void ADoorProgressFloodLights::TurnOnLight_Implementation(UStaticMeshComponent* StaticMeshComponentLight,
	int32 IndexToTurnOn)
{
	_aLightDatas[IndexToTurnOn].IsTurnedOn = true;
	indexToCheck++;
}



// Called every frame
void ADoorProgressFloodLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

