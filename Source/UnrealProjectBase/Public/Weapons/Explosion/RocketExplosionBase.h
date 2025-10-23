// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ProjectileInfluencable.h"
#include "Interfaces/DecalSpawnable.h"
#include "RocketExplosionBase.generated.h"


class ACharacterBaseFrog;
UCLASS()
class UNREALPROJECTBASE_API ARocketExplosionBase : public AActor,
 public IProjectileInfluencable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketExplosionBase();

	void DestroyAfterImpact_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable, Category = "Decal Trace Point")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable, Category = "Decal Trace Point")
	TObjectPtr<USceneComponent> SceneComponentTracePoint;
	

	UFUNCTION (BlueprintNativeEvent)
	void SpawnParticles();

	
	UFUNCTION (BlueprintNativeEvent)
	void SetRadiusOrWidithOfCollisionDamage();

	UPROPERTY()
	float _mRadiusOrWidithOfCollisionDamage;

	UFUNCTION (BlueprintNativeEvent)
	void SetRadiusOrWidithOfCollisionForce();

	UPROPERTY()
	float _mRadiusOrWidithOfCollisionForce;

	UFUNCTION (BlueprintNativeEvent)
	void SetRadiusOrWidithOfCollisionSense();

	UPROPERTY()
	float _mRadiusOrWidithOfCollisionSense;

	UFUNCTION (BlueprintNativeEvent)
	void SetLocationOfCollision();

	UPROPERTY()
	FVector _mLocationOfCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ObjectTypes")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ObjectTypes")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForceAndSense;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ObjectTypes")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesDamageOnly;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ObjectTypes")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesProjectile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ObjectTypes")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeApplyForceInDamageSphere;

	UPROPERTY(BlueprintReadWrite)
	bool _mIsEnemyInVolumeForce = false;

	UPROPERTY(BlueprintReadWrite)
	bool _mIsEnemyInVolumeDamge = false;

	UPROPERTY(BlueprintReadWrite)
	bool _mIsEnemyInVolumeSence = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ForceAppliedByForceCollision")
	float _mForce = 25000.0f;

	


	


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UFUNCTION(BlueprintNativeEvent)
	TArray<AActor*> GetOverlappingActorsInCollision(bool& InReturnValue, FVector Location, float RadiusOrWidth, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Stats")
	float _mDamage = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Stats")
	float _mLaunchForce = 2200.0f;

	UFUNCTION(BlueprintNativeEvent)
	void ApplyHitStopOnGameMode(float CountEnemiesDamaged);

	UFUNCTION(BlueprintNativeEvent)
	void ApplyDamageToCurrentIteration();

	UFUNCTION()
	void IterateOverEnemiesCollisionSense();

	UFUNCTION()
	void IterateOverEnemiesCollisionForce();

	UFUNCTION(BlueprintNativeEvent)
	void IterateOverActorsCollisionDamage();

	UFUNCTION()
	void ApplyDamageIfConditionsMet(AActor* ActorInlcude);

	UFUNCTION()
	void IteratePinnerProjectileAndDestroy();

	UFUNCTION()
	void CheckIfEnemyBehindCover(AActor* EnemyActor);

	UFUNCTION()
	void CheckTraceMiddle();

	UFUNCTION()
	void CheckTraceRight();

	UFUNCTION()
	void CheckTraceLeft();
	

	UPROPERTY()
	bool _mIsEffectedByCover = true;

	UPROPERTY()
	bool _mIsDamageOnlyAppliedWhileAirBorn = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace If Behind Cover")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;

	UPROPERTY()
	AActor* EnemyActorInCurrentIteration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace If Behind Cover")
	int32 _mTraceLength = 100;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace If Behind Cover")
	int32 _mAmountOffsetToLeftAndRight = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mZOffsetLeftAndRight = FVector(0,0,50);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mXOffsetLeftAndRight;
	
	UPROPERTY(BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mRightTraceEnd;
	
	UPROPERTY(BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mForwardVectorScaled;
	
	UPROPERTY(BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mAllTraceEnd;
	
	UPROPERTY(BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mLeftTraceStart;
	
	UPROPERTY(BlueprintReadWrite, Category="Trace If Behind Cover")
	FVector _mRightTraceStart;
	
	

};
