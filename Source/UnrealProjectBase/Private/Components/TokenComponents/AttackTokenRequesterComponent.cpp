// Copyright Peasant Pals 2025


#include "Components/TokenComponents/AttackTokenRequesterComponent.h"

#include "Interfaces/Token/AttackTokenGiverable.h"

// Sets default values for this component's properties
UAttackTokenRequesterComponent::UAttackTokenRequesterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAttackTokenRequesterComponent::ClearTokenMap()
{
	_mmTakenAttackTokens.Empty();
}

FVector UAttackTokenRequesterComponent::RequestAttackLocation_Implementation(AActor* AttackTarget)
{
	if (AttackTarget)
	{
		FRequesterGeneralTokenData& RequesterGeneralTokenData = _mmTakenAttackTokens.FindOrAdd(AttackTarget);

		if (RequesterGeneralTokenData.CountTokensTaken >= 1)
		{
			if (AttackTarget->Implements<UAttackTokenGiverable>())
			{
		
				RequesterGeneralTokenData.LocationOfReservedPoint = IAttackTokenGiverable::Execute_HandleAttackLocationRequest(AttackTarget, RequesterGeneralTokenData.IndexOfReservedPoint);
			}
		}
		return RequesterGeneralTokenData.LocationOfReservedPoint;
	}
	return FVector::ZeroVector;
	
}


bool UAttackTokenRequesterComponent::HasAttackTokens_Implementation(AActor* AttackTarget)
{
	if (AttackTarget)
	{
		FRequesterGeneralTokenData RequesterGeneralTokenData = _mmTakenAttackTokens.FindOrAdd(AttackTarget);
		
		if (RequesterGeneralTokenData.CountTokensTaken >= 1)
		{
			return true;
		}
	}
	return false;
}

void UAttackTokenRequesterComponent::ReturnAllAttackTokensForAllTargets_Implementation()
{
	if (_mmTakenAttackTokens.IsEmpty() == false)
	{
		for (TPair<AActor*, FRequesterGeneralTokenData> Pair: _mmTakenAttackTokens)
		{
			if (!Pair.Key) continue;
			if (Pair.Value.CountTokensTaken >= 1)
			{
				ReturnAttackTokens(Pair.Key);
			}
		}
	}
}




void UAttackTokenRequesterComponent::ReturnAttackTokens_Implementation(AActor* AttackTarget)
{
	if (!AttackTarget) return;
	
	FRequesterGeneralTokenData& RequesterGeneralTokenData = _mmTakenAttackTokens.FindOrAdd(AttackTarget);
	//Return token to Giver
	if (AttackTarget->Implements<UAttackTokenGiverable>())
	{
		IAttackTokenGiverable::Execute_HandleReturningToken(AttackTarget, RequesterGeneralTokenData.CountTokensTaken,
			RequesterGeneralTokenData.IndexOfReservedPoint);
	}
	//Remove token from requester map
	if (RequesterGeneralTokenData.CountTokensTaken > 0)
	{
		RequesterGeneralTokenData.CountTokensTaken =
			RequesterGeneralTokenData.CountTokensTaken - RequesterGeneralTokenData.TokensRequiredForAction;
		RequesterGeneralTokenData.LocationOfReservedPoint = FVector::ZeroVector;
	}
}


void UAttackTokenRequesterComponent::ReserveAttackTokenAndStoreInMap_Implementation(
	AActor* AttackTarget, int32 InIndexOfReservedPoint, FVector InLocationOfReservedPoint)
{
	if (!AttackTarget) return;
	
	FRequesterGeneralTokenData& RequesterGeneralTokenData =
		_mmTakenAttackTokens.FindOrAdd(AttackTarget);
	
	RequesterGeneralTokenData.LocationOfReservedPoint = InLocationOfReservedPoint;
	RequesterGeneralTokenData.IndexOfReservedPoint = InIndexOfReservedPoint;
	RequesterGeneralTokenData.CountTokensTaken =
		RequesterGeneralTokenData.CountTokensTaken
		+ RequesterGeneralTokenData.TokensRequiredForAction;
}

bool UAttackTokenRequesterComponent::RequestAttackToken_Implementation(AActor* AttackTarget)
{
	bool IsTokenRequestSuccessful = false;
	if (AttackTarget)
	{
		FRequesterGeneralTokenData& RequesterGeneralTokenData = _mmTakenAttackTokens.FindOrAdd(AttackTarget);
		FRequesterReservationData ReservationData = FRequesterReservationData();

		if (_maRequestedTokenIndexes.IsEmpty() == true) return false;
		if (!AttackTarget)return false;
		
		if (AttackTarget->Implements<UAttackTokenGiverable>())
		{
			IsTokenRequestSuccessful = IAttackTokenGiverable::Execute_HandleResevereTokenRequest(AttackTarget,
				RequesterGeneralTokenData, _maRequestedTokenIndexes, ReservationData);
			if (IsTokenRequestSuccessful == true)
			{
				ReserveAttackTokenAndStoreInMap(AttackTarget,
					ReservationData.IndexOfReservedPoint, ReservationData.LocationOfReservedPoint);
			}
		}
	}
	return IsTokenRequestSuccessful;
}




