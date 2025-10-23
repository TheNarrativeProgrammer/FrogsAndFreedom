// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/ProjectileBase.h"
#include "ProjectileBaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECTBASE_API AProjectileBaseEnemy : public AProjectileBase
{
	GENERATED_BODY()

	public:
	AProjectileBaseEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Body = nullptr;
};
