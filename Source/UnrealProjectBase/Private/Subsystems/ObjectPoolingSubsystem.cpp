// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ObjectPoolingSubsystem.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Characters/Frogs/CharacterBaseFrog.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Poolable.h"

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
	if (!ActorQueueWrapper->ActorsInPool.IsEmpty())
	{
		AActor* pooledActor = nullptr;
		if (_mdActorPools.FindOrAdd(poolableClass)->ActorsInPool.Dequeue(pooledActor))
		{
			if (!pooledActor) {return SpawnNewActor(poolableClass, spawnTransform);;}
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

			pooledActor->SetActorTransform(spawnTransform);
			pooledActor->SetActorLocation(spawnTransform.GetLocation());

			FQuat SafeQuat = spawnTransform.GetRotation();
			SafeQuat.Normalize();
			if (SafeQuat.ContainsNaN())
			{
				SafeQuat = FQuat::Identity;
			}
			pooledActor->SetActorRotation(SafeQuat);
			pooledActor->SetActorEnableCollision(true);
			pooledActor->SetActorTickEnabled(true);
			//the skeletal mesh is occcasionally invisible. This is a fix for it.
			
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
					CharacterMovementComponent->ResetMoveState();
				}
			}

			//SKELETAL MESH - Reactivate Physics, gravity and animation
			TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
			pooledActor->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

			if (SkeletalMeshComponents.IsEmpty() == false)
			{
				for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
				{
					if (SkeletalMeshComponent)
					{
						SkeletalMeshComponent->bPauseAnims = false;
						SkeletalMeshComponent->GlobalAnimRateScale = 1.0f;
						SkeletalMeshComponent->bNoSkeletonUpdate = false;
						SkeletalMeshComponent->SetVisibility(true);

						SkeletalMeshComponent->InitAnim(true);
						SkeletalMeshComponent->RefreshBoneTransforms();
						SkeletalMeshComponent->TickAnimation(0.0f, false);
						
						SkeletalMeshComponent->SetSimulatePhysics(false);
						SkeletalMeshComponent->SetEnableGravity(true);
						
						SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
						
						//the skeletal mesh is occcasionally invisible. This is a fix for it.
						SkeletalMeshComponent->SetVisibility(true);
					}

					if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
					{
						AnimInstance->Montage_Play(nullptr);
					
					}
				}
			}
			

			//SKELETAL MESH - Reactivate Physics, gravity and animation
			TArray<UStaticMeshComponent*> StaticMeshComponents;
			pooledActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

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
			pooledActor->GetComponents<UMaterialBillboardComponent>(MaterialBillboardComponents);

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
			
			

			if (APawn* pooledPawn = Cast<APawn>(pooledActor))
			{
				//stop movement of ai controller
				if (AController* Controller = pooledPawn->GetController())
				{
					//if a controller already exists, then unpossess before spawning default
					Controller->UnPossess();
				}
				if (!pooledPawn->GetController())
				{
					pooledPawn->SpawnDefaultController();
				}
				if (AController* Controller = pooledPawn->GetController())
				{
					
					if (AAIController* AIController = Cast<AAIController>(Controller))
					{
						AIController->SetActorTickEnabled(true);
						AIController->PrimaryActorTick.bCanEverTick = true;
						if (AIController)
						{
							AIController->BrainComponent->StartLogic();
						}
					}
				}
			}
			
			
			IPoolable::Execute_OnPoolActorActivated(pooledActor);
			_mCountActorsInGame++;
			return pooledActor;
		}
	}
	return SpawnNewActor(poolableClass, spawnTransform);
}

AActor* UObjectPoolingSubsystem::SpawnNewActor(TSubclassOf<AActor> poolableClass, const FTransform spawnTransform)
{
	//if no pool of this class type exist, or if zero actors remain in pool, then spawn a new actor
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* newActor = GetWorld()->SpawnActor<AActor>(poolableClass, spawnTransform, spawnParams);
	if (newActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: SpawnActorFromPool was empty or didn't have actor of class. New Actor of class spawned"));

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
		

		//SKELETAL MESH - Reactivate Physics, gravity and animation
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
		
		
	//_mdActorsInGamePlay.FindOrAdd(poolableClass).ActorsInPoolOrLevel.Add(newActor);
	TUniquePtr<FActorSetWrapper>* ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);

		if (!ActorSetWrapperPtr)
		{
			_mdActorsInGamePlay.Add(poolableClass, MakeUnique<FActorSetWrapper>());
			ActorSetWrapperPtr = _mdActorsInGamePlay.Find(poolableClass);
		}

		FActorSetWrapper* ActorSetWrapper = ActorSetWrapperPtr->Get();
		ActorSetWrapper->ActorsInGamePlay.Add(newActor);

		if (APawn* newPawn = Cast<APawn>(newActor) )
		{
			if (!newPawn->GetController())
			{
				//UE_LOG(LogTemp, Warning, TEXT("PROGRAMMER: Actor is was NOT possessed by controller. A default controller was spawned"));
				//Spawn controller and posses pawn
				newPawn->SpawnDefaultController();
			}
			newPawn->Restart();

			//stop movement of ai controller
			if (AController* Controller = newPawn->GetController())
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

	

	

	//Clear timers
	actorToReturn->GetWorldTimerManager().ClearAllTimersForObject(actorToReturn);

	//Lookup pool transform from map. Set Transform, location, lifespan and disable collision and tick. Call interface function for deactivation
	
	if (_mdPoolTransform.Contains(actorToReturn->GetClass()))
	{
		FTransform& poolTransform = _mdPoolTransform.FindOrAdd(actorToReturn->GetClass());
		poolTransform.NormalizeRotation();
		actorToReturn->SetActorTransform(poolTransform);
		actorToReturn->SetActorLocation(poolTransform.GetLocation());
	}
	
	
	actorToReturn->SetActorEnableCollision(false);
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
		//stop movement of ai controller
		if (AController* Controller = Character->GetController())
		{
			if (AAIController* AIController = Cast<AAIController>(Controller))
			{
				AIController->StopMovement();

				if (AIController->BrainComponent)
				{
					AIController->BrainComponent->StopLogic(TEXT("Paused Brain on REturn to pool"));
				}
				AIController->UnPossess();
				AIController->SetActorTickEnabled(false);
				AIController->PrimaryActorTick.bCanEverTick = false;
				
			}
			else
			{
				Controller->UnPossess();
				Controller->SetActorTickEnabled(false);
				Controller->PrimaryActorTick.bCanEverTick = false;
			}
		}
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

	if (SkeletalMeshComponents.IsEmpty() == false)
	{
		for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
		{
			if (!SkeletalMeshComponent)
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
				

				if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
				{
					AnimInstance->StopAllMontages(0.0f);
					//AnimInstance->Montage_Stop(0.0f);
				}
			}
		}
	}
	

	//STATIC MESH - Disable Physics and Gravity
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	actorToReturn->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	if (StaticMeshComponents.IsEmpty() == false)
	{
		for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
		{
			if (!StaticMeshComponent)
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
	}

	//Billboard Visibility
	TArray<UMaterialBillboardComponent*> MaterialBillboardComponents;
	actorToReturn->GetComponents<UMaterialBillboardComponent>(MaterialBillboardComponents);

	if (MaterialBillboardComponents.IsEmpty() == false)
	{
		for (UMaterialBillboardComponent* MaterialBillboardComponent : MaterialBillboardComponents)
		{
			if (MaterialBillboardComponent)
			{
				MaterialBillboardComponent->SetVisibility(false);
			}
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
			if (Actor->HasAnyFlags(RF_BeginDestroyed))
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
