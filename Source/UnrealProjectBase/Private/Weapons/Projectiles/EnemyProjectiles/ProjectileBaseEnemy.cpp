// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/EnemyProjectiles/ProjectileBaseEnemy.h"

AProjectileBaseEnemy::AProjectileBaseEnemy()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	SetRootComponent(Body);
}
