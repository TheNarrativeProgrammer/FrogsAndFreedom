// Copyright Peasant Pals 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/TokenData.h"
#include "AttackTokenGiverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECTBASE_API UAttackTokenGiverComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAttackTokenGiverComponent();
	void InitializeComponent() override;
	
	UPROPERTY(EditDefaultsOnly, Category="AttackTokenGiverComponent")
	int32 _mTotalAttackTokensFlusher;

	UPROPERTY(EditDefaultsOnly, Category="AttackTokenGiverComponent")
	int32 _mTotalAttackTokensPinner;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AttackLocations Array")
	TMap<int32, FGiverAttackTokenData> _mmTokenDataAttackLocations;

	UFUNCTION(BlueprintCallable)
	bool HandleResevereTokenRequest(const FRequesterGeneralTokenData& RequesterTokenData,
		const TArray<int32> RequestedIndexes, FRequesterReservationData& RequesterReservationData);
	
	UFUNCTION(BlueprintCallable)
	void HandleReturningToken(int32 InCountTokensTaken, int32 InIndexOfReservedPoint);

	UFUNCTION(BlueprintCallable)
	FVector HandleAttackLocationRequest(int32 InIndexOfReservedPoint);

	UFUNCTION()
	void AddAttackLocationToMap(int32 Key, USceneComponent* SceneComponentAttackLocation,
		bool InIsTokenAvailable, int32 TokensAvailable);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationLeftFlusher;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationMidFlusher;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationRightFlusher;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationLeftPinner;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationMidPinner;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponentEnemyAttackLocationRightPinner;
	
	
	
	

	
	

		
};
