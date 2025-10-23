// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/DelegateBindBroadcastable.h"
#include "Components/BindComponents/DelegateBindComponentLevelDoor.h"
#include "LevelDoorBase.generated.h"

UCLASS()
class UNREALPROJECTBASE_API ALevelDoorBase : public AActor, public IDelegateBindBroadcastable 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelDoorBase();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDelegateBindComponentLevelDoor> DelegateBindingComponentLevelDoor;
	
	
	//PROPERTIES - components
	UPROPERTY(EditDefaultsOnly, Blueprintable, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	
	// FUNCTIONS - begin play
	virtual void BeginPlay() override;

	//Inherited Functions - IDelegateBindBroadcastable
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginplayBroadcastActorInitializationDelegates();

	
public:	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenDoor();

	

};
