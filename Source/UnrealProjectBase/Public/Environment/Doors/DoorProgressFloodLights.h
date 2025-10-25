// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorProgressFloodLights.generated.h"

USTRUCT(BlueprintType)
struct FLightData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Light Properties")
	UStaticMeshComponent* StaticMeshComponentLight = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Light Properties")
	bool IsTurnedOn = false;

	UPROPERTY(BlueprintReadOnly, Category="Light Properties")
	float TimeToTurnOnLight = 0.0f;

	FORCEINLINE bool operator ==(const FLightData& Other) const
	{
		return StaticMeshComponentLight == Other.StaticMeshComponentLight;
	}
};
UCLASS()
class UNREALPROJECTBASE_API ADoorProgressFloodLights : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorProgressFloodLights();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Door")
	TArray<FLightData> _aLightDatas;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Door")
	int32 indexToCheck = 0;

	UFUNCTION(BlueprintCallable, Category = "Light Array")
	void AddLightToLightDatas(UStaticMeshComponent* StaticMeshComponentLight);

	UFUNCTION(BlueprintCallable, Category = "Light Array")
	void AssignTimeToTurnOnLight(float TimeLengthObjectiveGeneratorDefence);

	UFUNCTION(BlueprintCallable, Category = "Light Array")
	void CheckIfLightCanTurnOn(float RemainingTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Light Event")
	void TurnOnLight(UStaticMeshComponent* StaticMeshComponentLight, int32 IndexToTurnOn);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
