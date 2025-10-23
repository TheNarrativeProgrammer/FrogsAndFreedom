// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ObjectPoolingSubsystem.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Poolable.h"
#include "AIController.h"
#include "UObject/FastReferenceCollector.h"

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

void UObjectPoolingSubsystem::PreWarmPool(TSubclassOf<AActor> poolableClass, int32 count, FTransform poolTransform)
{
	if (!IsValid(poolableClass)
		|| !poolableClass->ImplementsInterface(UPoolable::StaticClass())
		|| count < 1
		|| !poolableClass->IsChildOf(APawn::StaticClass()))
	{
		return;
	}

	//Initialize pools with class key and values
	//FActorQueueWrapper& PoolByClass = _mdActorPools.FindOrAdd(poolableClass);
	_mdPoolTransform.FindOrAdd(poolableClass)=poolTransform;

	for (int32 i = 0; i < count; i++)
	{
		//Spawn Actor at pool location
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* newActor = GetWorld()->SpawnActor<AActor>(poolableClass, poolTransform, spawnParams);

		//Add newly created actor to pool map.
		if (newActor)
		{
			//note: this line double adds actors to _mdActorPool since ReturnToPool also adds actor to pool.
			//_mdActorPools.Find(poolableClass)->ActorsInPoolOrLevel.Add(newActor);
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
		// Warning, TEXT("PROGRAMMER: SpawnActorFromPool passed a poolableClass that's invalid"));
		return nullptr;
	}

	if (!poolableClass->ImplementsInterface(UPoolable::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: SpawnActorFromPool passed a poolableClass that's doesn't implement Poolable interface"));
		return nullptr;
	}

	//Pass key, Class of actor, into map and get the value, Array of actors in pool of that class
	//FActorQueueWrapper& poolByClass = _mdActorPools.FindOrAdd(poolableClass);

	TUniquePtr<FActorQueueWrapper>* ActorQueueWrapperPtr = _mdActorPools.Find(poolableClass);

	//If the queue of this class doesn't already exist, then create a new one
	if (!ActorQueueWrapperPtr)
	{
		_mdActorPools.Add(poolableClass, MakeUnique<FActorQueueWrapper>());
		ActorQueueWrapperPtr = _mdActorPools.Find(poolableClass);
	}

	FActorQueueWrapper* ActorQueueWrapper = ActorQueueWrapperPtr->Get();

	//Pop actor out of Pool and add it to the map tracking actors in game play.
	//Set Transform, location, Colluision, Tick, and RegisterComponents
	//check if pooled actor inherits from pawn and if controller is attached. manually trigger possession of controller via setting default.
	// if (poolByClass.ActorsInPoolOrLevel.Num() > 4)
	if (ActorQueueWrapper->ActorsInPool.IsEmpty() == false)
	{
		AActor* pooledActor = nullptr;
		if (_mdActorPools.FindOrAdd(poolableClass)->ActorsInPool.Dequeue(pooledActor))
		{
			//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: SpawnActorFromPool has Actor in pool"));
			//_mdActorsInGamePlay.FindOrAdd(poolableClass)->ActorsInGamePlay.Add(pooledActor);

			//_mdActorsInGamePlay.FindOrAdd(poolableClass).ActorsInPoolOrLevel.Add(newActor);
			TUniquePtr<FActorSetWrapper>* ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);

			if (!ActorSetWrapperPtr)
			{
				_mdActorsInGamePlay.Add(poolableClass, MakeUnique<FActorSetWrapper>());
				ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);
			}

			FActorSetWrapper* ActorSetWrapper = ActorSetWrapperPtr->Get();
			ActorSetWrapper->ActorsInGamePlay.Add(pooledActor);

			//SKELETAL MESH - Reactivate Physics, gravity and animation
			TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
			pooledActor->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

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
					SkeletalMeshComponent->bNoSkeletonUpdate = false;
					//the skeletal mesh is occcasionally invisible. This is a fix for it.
					SkeletalMeshComponent->SetVisibility(true);
				}

				if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
				{
					AnimInstance->Montage_Play(nullptr);
					
				}
			}

			//SKELETAL MESH - Reactivate Physics, gravity and animation
			TArray<UStaticMeshComponent*> StaticMeshComponents;
			pooledActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

			for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
			{
				if (StaticMeshComponent)
				{
					StaticMeshComponent->SetSimulatePhysics(true);
					StaticMeshComponent->SetEnableGravity(true);
					StaticMeshComponent->SetVisibility(true);
				}
			}
			

			//stop movement of ai controller
			if (AController* Controller = pooledActor->GetInstigatorController())
			{
				if (AAIController* AIController = Cast<AAIController>(Controller))
				{
					//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: Actor is possessed by controller and it's logic is started"));
					AIController->Possess(Cast<APawn>(pooledActor));
					AIController->SetActorTickEnabled(true);
					AIController->PrimaryActorTick.bCanEverTick = true;
					if (AIController->BrainComponent)
					{
						AIController->BrainComponent->StartLogic();
					}
				}
			}
			
			else if (APawn* pooledPawn = Cast<APawn>(pooledActor) )
			{
				if (!pooledPawn->GetController())
				{
					//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: Actor is was NOT possessed by controller. A default controller was spawned"));
					//Spawn controller and posses pawn
					pooledPawn->SpawnDefaultController();
				}
				pooledPawn->Restart();
				
				
			}

			pooledActor->SetActorTransform(spawnTransform);
			pooledActor->SetActorLocation(spawnTransform.GetLocation());
			pooledActor->SetActorRotation(spawnTransform.GetRotation());
			pooledActor->SetActorEnableCollision(true);
			pooledActor->SetActorTickEnabled(true);
			//the skeletal mesh is occcasionally invisible. This is a fix for it.
			pooledActor->GetComponentByClass<USkeletalMeshComponent>()->SetVisibility(true);
			pooledActor->RegisterAllComponents();

			if (UPrimitiveComponent* RootPrimativeComponent = Cast<UPrimitiveComponent>(pooledActor->GetRootComponent()))
			{
				RootPrimativeComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
			}
	
			if (UCapsuleComponent* CapsuleComponent = pooledActor->GetComponentByClass<UCapsuleComponent>())
			{
				CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
				CapsuleComponent->SetVisibility(true);
				CapsuleComponent->SetComponentTickEnabled(true);
			}

			if (ACharacter* Character = Cast<ACharacter>(pooledActor))
			{
				if (UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
				{
					CharacterMovementComponent->SetMovementMode(MOVE_Walking);
					CharacterMovementComponent->SetComponentTickEnabled(true);
				}
			}

			
			IPoolable::Execute_OnPoolActorActivated(pooledActor);
			_mCountActorsInGame++;
			return pooledActor;
		}
	}
	//if no pool of this class type exist, or if zero actors remain in pool, then spawn a new actor
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* newActor = GetWorld()->SpawnActor<AActor>(poolableClass, spawnTransform, spawnParams);
	if (newActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: SpawnActorFromPool was empty or didn't have actor of class. New Actor of class spawned"));

		newActor->SetActorTransform(spawnTransform);
		newActor->SetActorLocation(spawnTransform.GetLocation());
		newActor->SetActorRotation(spawnTransform.GetRotation());
		newActor->SetActorEnableCollision(true);
		newActor->SetActorTickEnabled(true);
		newActor->RegisterAllComponents();

		

		//SKELETAL MESH - Reactivate Physics, gravity and animation
		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		newActor->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

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

		//SKELETAL MESH - Reactivate Physics, gravity and animation
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		newActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

		for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
		{
			if (StaticMeshComponent)
			{
				StaticMeshComponent->SetSimulatePhysics(true);
				StaticMeshComponent->SetEnableGravity(true);
				StaticMeshComponent->SetVisibility(true);
			}
		}
		
	//_mdActorsInGamePlay.FindOrAdd(poolableClass).ActorsInPoolOrLevel.Add(newActor);
	TUniquePtr<FActorSetWrapper>* ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);

		if (!ActorSetWrapperPtr)
		{
			_mdActorsInGamePlay.Add(poolableClass, MakeUnique<FActorSetWrapper>());
			ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);
		}

		FActorSetWrapper* ActorSetWrapper = ActorSetWrapperPtr->Get();
		ActorSetWrapper->ActorsInGamePlay.Add(newActor);

		if (APawn* pooledPawn = Cast<APawn>(newActor) )
		{
			if (!pooledPawn->GetController())
			{
				//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: Actor is was NOT possessed by controller. A default controller was spawned"));
				//Spawn controller and posses pawn
				pooledPawn->SpawnDefaultController();
			}
			pooledPawn->Restart();
		}
		//stop movement of ai controller
		if (AController* Controller = newActor->GetInstigatorController())
		{
			if (AAIController* AIController = Cast<AAIController>(Controller))
			{
				//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: Actor is possessed by controller and it's logic is started"));
				AIController->Possess(Cast<APawn>(newActor));
				if (AIController->BrainComponent)
				{
					AIController->BrainComponent->StartLogic();
				}
			}
		}
	}
	//the skeletal mesh is occcasionally invisible. This is a fix for it.
	newActor->GetComponentByClass<USkeletalMeshComponent>()->SetVisibility(true);
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

	_mCountActorsInPool++;
	
	
	if (_mdActorsInGamePlay.FindOrAdd(actorToReturn->GetClass())->ActorsInGamePlay.Contains(actorToReturn))
	{
		_mdActorsInGamePlay.FindOrAdd(actorToReturn->GetClass())->ActorsInGamePlay.Remove(actorToReturn);
	}
	
	//Add actor to pool of it's class
	//FActorArrayWrapper& poolByClass = _mdActorPools.FindOrAdd(actorToReturn->GetClass());
	//poolByClass.ActorsInPoolOrLevel.Add(actorToReturn);

	TUniquePtr<FActorQueueWrapper>* ActorQueueWrapperPtr = _mdActorPools.Find(actorToReturn->GetClass());

	if (!ActorQueueWrapperPtr)
	{
		_mdActorPools.Add(actorToReturn->GetClass(), MakeUnique<FActorQueueWrapper>());
		ActorQueueWrapperPtr = _mdActorPools.Find(actorToReturn->GetClass());
	}

	ActorQueueWrapperPtr->Get()->ActorsInPool.Enqueue(actorToReturn);

	
	//_mdActorPools.FindOrAdd(actorToReturn->GetClass())->ActorsInPool.Enqueue(actorToReturn);

	//stop movement of ai controller
	if (AController* Controller = actorToReturn->GetInstigatorController())
	{
		if (AAIController* AIController = Cast<AAIController>(Controller))
		{
			AIController->StopMovement();
			AIController->SetActorTickEnabled(false);
			AIController->PrimaryActorTick.bCanEverTick = false;

			if (AIController->BrainComponent)
			{
				AIController->BrainComponent->StopLogic(TEXT("Brain Stopped. Returned to pool"));
			}
			//AIController->UnPossess();
		}
	}

	

	//Clear timers
	actorToReturn->GetWorldTimerManager().ClearAllTimersForObject(actorToReturn);

	//Lookup pool transform from map. Set Transform, location, lifespan and disable collision and tick. Call interface function for deactivation
	FTransform poolTransform = FTransform::Identity;
	if (_mdPoolTransform.Contains(actorToReturn->GetClass()))
	{
		poolTransform = _mdPoolTransform.FindOrAdd(actorToReturn->GetClass());
	}
	
	actorToReturn->SetActorTransform(poolTransform);
	actorToReturn->SetActorLocation(poolTransform.GetLocation());
	actorToReturn->SetActorEnableCollision(false);
	actorToReturn->SetActorTickEnabled(false);
	//actorToReturn->SetActorHiddenInGame(true);

	if (UPrimitiveComponent* RootPrimativeComponent = Cast<UPrimitiveComponent>(actorToReturn->GetRootComponent()))
	{
		RootPrimativeComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (UCapsuleComponent* CapsuleComponent = actorToReturn->GetComponentByClass<UCapsuleComponent>())
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		CapsuleComponent->SetVisibility(false);
		CapsuleComponent->SetComponentTickEnabled(false);
	}

	if (ACharacter* Character = Cast<ACharacter>(actorToReturn))
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
		{
			CharacterMovementComponent->StopMovementImmediately();
			CharacterMovementComponent->DisableMovement();
			CharacterMovementComponent->SetComponentTickEnabled(false);
		}
	}

	
	

	//SKELETAL MESH - Disable Physics, Gravity, and animations ticking
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	actorToReturn->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		if (!IsValid(SkeletalMeshComponent))
		{
			continue;
		}
		if (SkeletalMeshComponent)
		{
			//disable physics and gravity
			SkeletalMeshComponent->SetSimulatePhysics(false);
			SkeletalMeshComponent->SetEnableGravity(false);
			SkeletalMeshComponent->SetVisibility(false);
			SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

			//stop animation
			SkeletalMeshComponent->GlobalAnimRateScale = 0.0f;
			SkeletalMeshComponent->bPauseAnims = true;
			SkeletalMeshComponent->bNoSkeletonUpdate = true;

			if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
			{
				AnimInstance->StopAllMontages(0.0f);
				//AnimInstance->Montage_Stop(0.0f);
			}
		}
	}

	//STATIC MESH - Disable Physics and Gravity
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	actorToReturn->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
	{
		if (!IsValid(StaticMeshComponent))
		{
			continue;
		}
		if (StaticMeshComponent)
		{
			StaticMeshComponent->SetSimulatePhysics(false);
			StaticMeshComponent->SetEnableGravity(false);
			StaticMeshComponent->SetVisibility(false);
			StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}
	}

	

	//IPoolable::Execute_OnPoolActorDeactivated(actorToReturn);
}

int32 UObjectPoolingSubsystem::GetTotalFrogsInGamePlayAllClasses()
{
	return _mCountActorsInGame;
}

int32 UObjectPoolingSubsystem::GetTotalFrogsInGamePlaySingleClass(TSubclassOf<AActor> frogClass)
{
	int32 totalFrogsOfClass = 0;
	//TSubclassOf<AActor> frogActorClass = *frogClass;
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
				//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: OnLevelLoadReturnCurrentActorsInLevelToPool tried returning null actor to pool"));
				continue;
			}

			AActor* Actor = WeakActorPtr.Get();
			if (Actor->HasAnyFlags(RF_BeginDestroyed) == true)
			{
				//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: OnLevelLoadReturnCurrentActorsInLevelToPool tried returning actor marked for garbage"));
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
		while (!Pair.Value->ActorsInPool.IsEmpty() == false)
		{
			AActor* Actor = nullptr;
			Pair.Value->ActorsInPool.Dequeue(Actor);
			if ( IsValid(Actor) == false ||  Actor == nullptr)
			{
				continue;
			}
			else if (HasAnyFlags(RF_BeginDestroyed) == true)
			{
				continue;;
			}
			Actor->Destroy();
		}
	}
	_mdActorPools.Empty();
}

void UObjectPoolingSubsystem::OnDeathClearPoolArrays()
{
	_mdActorPools.Empty();
	_mdActorsInGamePlay.Empty();
}

void UObjectPoolingSubsystem::ReturnAllGamePlayActorsToPool()
{
	TArray<TSubclassOf<AActor>> Keys;
	_mdActorsInGamePlay.GetKeys(Keys);

	TArray<AActor*> ActorsToMove;
	for (const TSubclassOf<AActor>& ActorClassKey : Keys)
	{
		//make array to hold Actors being removed from Gameplaymap
		TUniquePtr<FActorSetWrapper>* WrapperPtr = _mdActorsInGamePlay.Find(ActorClassKey);

		if (WrapperPtr && WrapperPtr->IsValid())
		{
			//ActorsToMove.Append(_mdActorsInGamePlay.Find(ActorClassKey)->ActorsInGamePlay.Array());
			ActorsToMove.Append(WrapperPtr->Get()->ActorsInGamePlay.Array());
		}
	}

	for (AActor* Actor : ActorsToMove)
	{
		ReturnToPool(Actor);
	}
}
