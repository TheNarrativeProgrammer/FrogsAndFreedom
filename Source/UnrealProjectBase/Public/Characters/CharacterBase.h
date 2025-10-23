// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "CharacterBase.generated.h"


UCLASS()
class UNREALPROJECTBASE_API ACharacterBase : public ACharacter,
public IGameplayTagAssetInterface,
public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	ACharacterBase();
	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }

	
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	

	//Property - IGenericTeamAgentInterface
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	// int32 IdNumber;

	
	

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGenericTeamId TeamId;
	

	//TODO check if UhealthComponent should be Blueprintable or Edited defaults only. Rohits notes say Edits Defaults, but
	//may need to be blueprintable
	
	// UPROPERTY(Blueprintable, BlueprintReadOnly)
	// TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlayTagContainer")
	FGameplayTagContainer GameplayTagContainer;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	

	//Function - inherits from IGenericTeamAgentInterface. IdNumber is public and is used to change the TeamId variable
	//virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent();

};
