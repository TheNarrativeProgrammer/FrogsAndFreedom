// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ObjectPoolingSubsystem.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Characters/Frogs/Enemies/CharacterBaseFrogTank.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Poolable.h"
#include "Core/GameStateEventManagerBase.h"

class ADetourCrowdAIController;

bool UObjectPoolingSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	Super::ShouldCreateSubsystem(Outer);
	return true;
}

void UObjectPoolingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void UObjectPoolingSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UObjectPoolingSubsystem::StopPoolingOnceKillAllFrogsObjectiveStarts()
{
	shouldStopPooling = true;
}

void UObjectPoolingSubsystem::BindDelegates()
{
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>(
		UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
		GameStateEventManager->OnKillAllFrogsStartedSignature.AddUniqueDynamic(this, &UObjectPoolingSubsystem::StopPoolingOnceKillAllFrogsObjectiveStarts);
	}
}

void UObjectPoolingSubsystem::UnBindDelegates()
{
	if (AGameStateEventManagerBase* GameState = Cast<AGameStateEventManagerBase>(
		UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStateEventManager = GameState;
		GameStateEventManager->OnKillAllFrogsStartedSignature.RemoveDynamic(this, &UObjectPoolingSubsystem::StopPoolingOnceKillAllFrogsObjectiveStarts);
	}
}

void UObjectPoolingSubsystem::PreWarmPool(TSubclassOf<AActor> poolableClass, int32 count, FTransform poolTransform)
{
	
	
	if (!IsValid(poolableClass)
		|| !poolableClass->ImplementsInterface(UPoolable::StaticClass())
		|| count < 1
		|| !poolableClass->IsChildOf(APawn::StaticClass()))
	{
		return;
	}
	
	_mdPoolTransform.FindOrAdd(poolableClass)=poolTransform;

	for (int32 i = 0; i < count; i++)
	{
		//Spawn Actor at pool location
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* newActor = GetWorld()->SpawnActor<AActor>(poolableClass, poolTransform, spawnParams);
		
		if (newActor)
		{
			ReturnToPool(newActor);
		}
	}
	
	
}

AActor* UObjectPoolingSubsystem::SpawnActorFromPool(TSubclassOf<AActor> poolableClass, const FTransform spawnTransform)
{
	if (!IsValid(poolableClass)
		|| poolableClass == nullptr
		|| poolableClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated))
	{
		return nullptr;
	}

	if (!poolableClass->ImplementsInterface(UPoolable::StaticClass()))
	{
		return nullptr;
	}
	return SpawnNewActor(poolableClass, spawnTransform);
}

AActor* UObjectPoolingSubsystem::SpawnNewActor(TSubclassOf<AActor> poolableClass, const FTransform spawnTransform)
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* newActor = GetWorld()->SpawnActor<AActor>(poolableClass, spawnTransform, spawnParams);
	if (newActor)
	{
		newActor->SetActorTransform(spawnTransform);
		newActor->SetActorLocation(spawnTransform.GetLocation());

		FQuat SafeQuat = spawnTransform.GetRotation();
		SafeQuat.Normalize();
		if (SafeQuat.ContainsNaN())
		{
			SafeQuat = FQuat::Identity;
		}
		newActor->SetActorRotation(SafeQuat);
		newActor->SetActorEnableCollision(true);
		newActor->SetActorTickEnabled(true);
		newActor->RegisterAllComponents();
		

		//SKELETAL MESH - Reactivate Physics, gravity and animation
		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		newActor->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

		if (SkeletalMeshComponents.IsEmpty() == false)
		{
			for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
			{
				if (SkeletalMeshComponent)
				{
					SkeletalMeshComponent->SetSimulatePhysics(false);
					SkeletalMeshComponent->SetEnableGravity(true);
					SkeletalMeshComponent->SetVisibility(true);
					SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
					SkeletalMeshComponent->bPauseAnims = false;
					SkeletalMeshComponent->GlobalAnimRateScale = 1.0f;
				}

				if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
				{
					AnimInstance->Montage_Play(nullptr);
				}
			}
		}
		

		//SKELETAL MESH - Reactivate Physics, gravity and animation. Do twice because of unknown bug.
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		newActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

		if (StaticMeshComponents.IsEmpty() == false)
		{
			for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
			{
				if (StaticMeshComponent)
				{
					StaticMeshComponent->SetSimulatePhysics(true);
					StaticMeshComponent->SetEnableGravity(true);
					StaticMeshComponent->SetVisibility(true);
				}
			}
		}
		
		//Billboard Visibility
		TArray<UMaterialBillboardComponent*> MaterialBillboardComponents;
		newActor->GetComponents<UMaterialBillboardComponent>(MaterialBillboardComponents);

		if (MaterialBillboardComponents.IsEmpty() == false)
		{
			for (UMaterialBillboardComponent* MaterialBillboardComponent : MaterialBillboardComponents)
			{
				if (MaterialBillboardComponent)
				{
					MaterialBillboardComponent->SetVisibility(true);
				}
			}
		}
		
		
		TUniquePtr<FActorSetWrapper>* ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);
		
		if (APawn* newPawn = Cast<APawn>(newActor) )
		{
			if (!newPawn->GetController())
			{
				newPawn->SpawnDefaultController();
			}
			newPawn->Restart();

			//stop movement of ai controller
			if (AController* Controller = newPawn->GetController())
			{
				if (AAIController* AIController = Cast<AAIController>(Controller))
				{
					AIController->Possess(Cast<APawn>(newActor));
					if (AIController->BrainComponent)
					{
						AIController->BrainComponent->StartLogic();
					}
				}
			}
		}
	}

	
	//the skeletal mesh is occcasionally invisible. This is a fix for it.
	IPoolable::Execute_OnPoolActorActivated(newActor);
	_mCountActorsInGame++;
	return newActor;
}

void UObjectPoolingSubsystem::ReturnToPool(AActor* actorToReturn)
{
	if (actorToReturn == nullptr
		|| IsValid(actorToReturn) == false
		|| actorToReturn->HasAnyFlags(RF_BeginDestroyed) == true
		|| actorToReturn->Implements<UPoolable>() == false)
		return;
	
	IPoolable::Execute_OnPoolActorDeactivated(actorToReturn);
	_mCountActorsInGame--;
	actorToReturn->Destroy();
}



int32 UObjectPoolingSubsystem::GetTotalFrogsInGamePlayAllClasses()
{
	return _mCountActorsInGame;
}

int32 UObjectPoolingSubsystem::GetTotalFrogsInGamePlaySingleClass(TSubclassOf<AActor> frogClass)
{
	int32 totalFrogsOfClass = 0;
	if (_mdActorsInGamePlay.Contains(frogClass))
	{
		totalFrogsOfClass = _mdActorsInGamePlay.FindOrAdd(frogClass, MakeUnique<FActorSetWrapper>())->ActorsInGamePlay.Num();
	}
	return totalFrogsOfClass;
}

void UObjectPoolingSubsystem::OnLevelLoadReturnCurrentActorsInLevelToPool()
{
	for (TPair<TSubclassOf<AActor>, TUniquePtr<FActorSetWrapper>>& Pair : _mdActorsInGamePlay)
	{
		if (!IsValid(Pair.Key))
		{
			continue;
		}
		TArray<AActor*> ActorsInGamePlayCopy = Pair.Value->ActorsInGamePlay.Array();
	
		for (TWeakObjectPtr<AActor> WeakActorPtr : ActorsInGamePlayCopy)
		{
			if ( WeakActorPtr.IsValid() == false)
			{
				continue;
			}

			AActor* Actor = WeakActorPtr.Get();
			if (Actor->HasAnyFlags(RF_BeginDestroyed))
			{
				continue;
			}
			ReturnToPool(Actor);
		}
	}
	_mdActorsInGamePlay.Empty();
}

void UObjectPoolingSubsystem::OffloadPool()
{
	for (TPair<TSubclassOf<AActor>, TUniquePtr<FActorQueueWrapper>>& Pair: _mdActorPools)
	{
		if (!IsValid(Pair.Key))
		{
			continue;
		}
		while (!Pair.Value->ActorsInPool.IsEmpty())
		{
			AActor* Actor = nullptr;
			Pair.Value->ActorsInPool.Dequeue(Actor);
			if (!IsValid(Actor))
			{
				continue;
			}
			if (Actor->HasAnyFlags(RF_BeginDestroyed))
			{
				continue;;
			}
			Actor->Destroy();
		}
	}
	
}

void UObjectPoolingSubsystem::OnDeathClearPoolArrays()
{
	_mdActorPools.Empty();
	_mdActorsInGamePlay.Empty();
}

void UObjectPoolingSubsystem::ReturnAllGamePlayActorsToPool()
{
	if (_mdActorPools.Num() == 0)
	{
		return;
	}
	TArray<TSubclassOf<AActor>> Keys;
	_mdActorsInGamePlay.GetKeys(Keys);

	TArray<AActor*> ActorsToMove;
	for (const TSubclassOf<AActor>& ActorClassKey : Keys)
	{
		TUniquePtr<FActorSetWrapper>* WrapperPtr = _mdActorsInGamePlay.Find(ActorClassKey);

		if (WrapperPtr && WrapperPtr->IsValid())
		{
			ActorsToMove.Append(WrapperPtr->Get()->ActorsInGamePlay.Array());
		}
	}

	for (AActor* Actor : ActorsToMove)
	{
		ReturnToPool(Actor);
	}
}
