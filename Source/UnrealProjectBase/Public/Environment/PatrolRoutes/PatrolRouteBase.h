// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "GameplayTagContainer.h"
#include "PatrolRouteBase.generated.h"

UCLASS()
class UNREALPROJECTBASE_API APatrolRouteBase : public AActor,
public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolRouteBase();

	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTag", meta = (Categories = "Environment.PatrolRoute.Type,Wave"))
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadWrite, Category = "PatrolRoute")
	TObjectPtr<USplineComponent> SpineComponentPatrolRoute;

	
	//Take in the index and direction variables of a frog and either set the index and direction or return a spline location.
	UFUNCTION(BlueprintCallable)
	void IncrumentPatrolRoute(int32& FrogPatrolIndex, int32& FrogDirection);

	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointAsWorldLocation(const int32& FrogPatrolIndex);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Route Variables")
	int32 _mPatrolIndex;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Route Variables", meta = (ClampMin = "1", ClampMax = "100", UIMin = "-1", UIMax = "1", UIMin = "-1", UIMax = "1"))
	int32 _mDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
