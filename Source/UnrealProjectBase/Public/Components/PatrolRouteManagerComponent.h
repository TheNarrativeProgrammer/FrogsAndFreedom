// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Environment/PatrolRoutes/PatrolRouteBase.h"
#include "PatrolRouteManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class UNREALPROJECTBASE_API UPatrolRouteManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolRouteManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Arrays of PatrolRoutes. Sorted based on GameplayTag
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<APatrolRouteBase*> _maPatrolRoutesFlusher;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Data Array")
	TArray<APatrolRouteBase*> _maPatrolRoutesPinner;

	//radius of the EQS query based on the size of the encounter box. Take the x axis or y axis, whichever is larger
	//This is passed in from EncounterManager
	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadOnly, Category="EQS")
	float _mRadiusOfEQSBaseOnEncounterSize;

	UPROPERTY()
	FGameplayTag GameplayTagFlusher;

	UPROPERTY()
	FGameplayTag GameplayTagPinner;

	

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetRadiusOfEQSBaseOnEncounterSize(float inRadiusOfEQSBaseOnEncounterSize);

	UFUNCTION(BlueprintCallable)
	void AddEncounterPatrolRoutesToArrays(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RunEqsFindPatrolRoutes();

	UFUNCTION(BlueprintCallable)
	void EmptyArraysPatrolRoutes();

	UFUNCTION(BlueprintCallable)
	void SetPatrolRouteOfBatch(const FEncounterSpawnData& EncounterSpawnData,int32 InIndexOfPatrolRoute, APatrolRouteBase*& InPatrolRouteBaseOfCurrentBatch);
		
};
