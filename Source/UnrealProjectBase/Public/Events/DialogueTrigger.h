// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DialogueTrigger.generated.h"

UCLASS()
class UNREALPROJECTBASE_API ADialogueTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialogue Tag", meta=(Categories="Narrative"))
	FGameplayTag GameplayTag;

	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapTriggerDialague(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	

};
