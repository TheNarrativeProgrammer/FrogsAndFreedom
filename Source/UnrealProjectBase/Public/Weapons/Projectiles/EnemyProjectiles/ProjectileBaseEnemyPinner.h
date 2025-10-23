// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/EnemyProjectiles/ProjectileBaseEnemy.h"
#include "Interfaces/ProjectileInfluencable.h"
#include "ProjectileBaseEnemyPinner.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API AProjectileBaseEnemyPinner : public AProjectileBaseEnemy,
public IProjectileInfluencable
{
	GENERATED_BODY()

	public:
	void DestroyAfterImpact_Implementation() override;
	
};
