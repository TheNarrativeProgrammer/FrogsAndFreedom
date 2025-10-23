// Copyright Peasant Pals 2025


#include "Components/TokenComponents/AttackTokenGiverComponent.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Interfaces/Token/AttackTokenRequesterable.h"


// Sets default values for this component's properties
UAttackTokenGiverComponent::UAttackTokenGiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	SceneComponentEnemyAttackLocationLeftFlusher = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationLeftFlusher"));
	SceneComponentEnemyAttackLocationMidFlusher = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationMidFlusher"));
	SceneComponentEnemyAttackLocationRightFlusher = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationRightFlusher"));
	
	SceneComponentEnemyAttackLocationLeftPinner = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationLeftPinner"));
	SceneComponentEnemyAttackLocationMidPinner = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationMidPinner"));
	SceneComponentEnemyAttackLocationRightPinner = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationRightPinner"));
	
	//
	//
	AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, true, 1);
	AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, true, 1);
	AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, true, 1);
	
	AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, true, 1);
	AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, true, 1);
	AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, true, 1);

	SceneComponentEnemyAttackLocationLeftFlusher->PrimaryComponentTick.bStartWithTickEnabled = false;
	SceneComponentEnemyAttackLocationMidFlusher->PrimaryComponentTick.bStartWithTickEnabled = false;
	SceneComponentEnemyAttackLocationRightFlusher->PrimaryComponentTick.bStartWithTickEnabled = false;
	SceneComponentEnemyAttackLocationLeftPinner->PrimaryComponentTick.bStartWithTickEnabled = false;
	SceneComponentEnemyAttackLocationMidPinner->PrimaryComponentTick.bStartWithTickEnabled = false;
	SceneComponentEnemyAttackLocationRightPinner->PrimaryComponentTick.bStartWithTickEnabled = false;

	SceneComponentEnemyAttackLocationLeftFlusher->PrimaryComponentTick.bCanEverTick = false;
	SceneComponentEnemyAttackLocationMidFlusher->PrimaryComponentTick.bCanEverTick = false;
	SceneComponentEnemyAttackLocationRightFlusher->PrimaryComponentTick.bCanEverTick = false;
	SceneComponentEnemyAttackLocationLeftPinner->PrimaryComponentTick.bCanEverTick = false;
	SceneComponentEnemyAttackLocationMidPinner->PrimaryComponentTick.bCanEverTick = false;
	SceneComponentEnemyAttackLocationRightPinner->PrimaryComponentTick.bCanEverTick = false;

	
	
}

void UAttackTokenGiverComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SceneComponentEnemyAttackLocationLeftFlusher->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SceneComponentEnemyAttackLocationMidFlusher->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SceneComponentEnemyAttackLocationRightFlusher->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SceneComponentEnemyAttackLocationLeftPinner->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SceneComponentEnemyAttackLocationMidPinner->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SceneComponentEnemyAttackLocationRightPinner->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
		if (_mTotalAttackTokensFlusher >= 3)
		{
			int32 RemainderTokens = _mTotalAttackTokensFlusher % 3;
			int32 TokensAllSlotsHave = _mTotalAttackTokensFlusher / 3;
	
			if (RemainderTokens == 0)
			{
				AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, true, TokensAllSlotsHave);
				AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, true, TokensAllSlotsHave);
				AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, true, TokensAllSlotsHave);
			}
			else if (RemainderTokens == 1)
			{
				AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, true, TokensAllSlotsHave + 1);
				AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, true, TokensAllSlotsHave);
				AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, true, TokensAllSlotsHave);
			}
			else if (RemainderTokens == 2)
			{
				AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, true, TokensAllSlotsHave + 1);
				AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, true, TokensAllSlotsHave + 1);
				AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, true, TokensAllSlotsHave);
			}
		}
	else if (_mTotalAttackTokensFlusher == 2)
		{
			AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, true, 1);
			AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, true, 1);
			AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, false, 0);
			UE_LOG(LogTemp, Error, TEXT("DESIGNER _mTotalAttackTokensFlusher has only 2 slots"));
		}
	else if (_mTotalAttackTokensFlusher == 1)
	{
		AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, true, 1);
		AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, false, 0);
		AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, false, 0);
		UE_LOG(LogTemp, Error, TEXT("DESIGNER _mTotalAttackTokensFlusher has only 1 slot"));
	}
	else if (_mTotalAttackTokensFlusher == 0)
	{
		AddAttackLocationToMap(0, SceneComponentEnemyAttackLocationLeftFlusher, false, 0);
		AddAttackLocationToMap(1, SceneComponentEnemyAttackLocationMidFlusher, false, 0);
		AddAttackLocationToMap(2, SceneComponentEnemyAttackLocationRightFlusher, false, 0);
		UE_LOG(LogTemp, Error, TEXT("DESIGNER _mTotalAttackTokensFlusher has zero slots"));
	}
	
		
	
	if (_mTotalAttackTokensPinner >= 3)
	{
		int32 RemainderTokens = _mTotalAttackTokensPinner % 3;
		int32 TokensAllSlotsHave = _mTotalAttackTokensPinner / 3;
	
		if (RemainderTokens == 0)
		{
			AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, true, TokensAllSlotsHave);
			AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, true, TokensAllSlotsHave);
			AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, true, TokensAllSlotsHave);
		}
		else if (RemainderTokens == 1)
		{
			AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, true, TokensAllSlotsHave + 1);
			AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, true, TokensAllSlotsHave);
			AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, true, TokensAllSlotsHave);
		}
		else if (RemainderTokens == 2)
		{
			AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, true, TokensAllSlotsHave + 1);
			AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, true, TokensAllSlotsHave + 1);
			AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, true, TokensAllSlotsHave);
		}
	}
	else if (_mTotalAttackTokensPinner == 2)
	{
		AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, true, 1);
		AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, true, 1);
		AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, false, 0);
		UE_LOG(LogTemp, Error, TEXT("DESIGNER _mTotalAttackTokensPinner has only 2 slots"));
	}
	else if (_mTotalAttackTokensPinner == 1)
	{
		AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, true, 1);
		AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, false, 0);
		AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, false, 0);
		UE_LOG(LogTemp, Error, TEXT("DESIGNER _mTotalAttackTokensPinner has only 1 slot"));
	}
	else if (_mTotalAttackTokensPinner == 0)
	{
		AddAttackLocationToMap(3, SceneComponentEnemyAttackLocationLeftPinner, false, 0);
		AddAttackLocationToMap(4, SceneComponentEnemyAttackLocationMidPinner, false, 0);
		AddAttackLocationToMap(5, SceneComponentEnemyAttackLocationRightPinner, false, 0);
		UE_LOG(LogTemp, Error, TEXT("DESIGNER _mTotalAttackTokensPinner has zero slots"));
	}
}

void UAttackTokenGiverComponent::AddAttackLocationToMap(int32 Key, USceneComponent* SceneComponentAttackLocation,
	bool InIsTokenAvailable, int32 TokensAvailable)
{
	if (!SceneComponentAttackLocation)
	{
		return;
	}
	
	//create struct
	FGiverAttackTokenData GiverAttackTokenData;
	GiverAttackTokenData.SceneComponentAttackLocation = SceneComponentAttackLocation;
	GiverAttackTokenData.IsTokenAvailable = InIsTokenAvailable;
	GiverAttackTokenData.TokensAvailableInSlot = TokensAvailable;
	//add struct to map
	_mmTokenDataAttackLocations.FindOrAdd(Key, GiverAttackTokenData);
}




bool UAttackTokenGiverComponent::HandleResevereTokenRequest(const FRequesterGeneralTokenData& RequesterTokenData,
	const TArray<int32> RequestedIndexes, FRequesterReservationData& RequesterReservationData)
{
	bool IsTokenRequestSuccessful = false;
		 for (int32 Index : RequestedIndexes)
		 {
		 	if (_mmTokenDataAttackLocations.Contains(Index) == false) return false;
		 	
		 	FGiverAttackTokenData& GiverAttackTokenData = *_mmTokenDataAttackLocations.Find(Index);
		 	
		 		if (GiverAttackTokenData.IsTokenAvailable && GiverAttackTokenData.TokensAvailableInSlot <= RequesterTokenData.TokensRequiredForAction)
		 		{
		 			//Update Giver Data
		 			GiverAttackTokenData.TokensAvailableInSlot = GiverAttackTokenData.TokensAvailableInSlot - RequesterTokenData.TokensRequiredForAction;
		 			if (GiverAttackTokenData.TokensAvailableInSlot <= 0)
		 			{
		 				GiverAttackTokenData.IsTokenAvailable = false;
		 			}
		 			//update Requester data
		 			RequesterReservationData.IndexOfReservedPoint = Index;
		 			RequesterReservationData.LocationOfReservedPoint = GiverAttackTokenData.SceneComponentAttackLocation->GetComponentLocation();
		
		 			IsTokenRequestSuccessful = true;
		 			return IsTokenRequestSuccessful;
		 		}
		 	}
	return IsTokenRequestSuccessful;
}

void UAttackTokenGiverComponent::HandleReturningToken(int32 InCountTokensTaken, int32 InIndexOfReservedPoint)
{
	FGiverAttackTokenData& GiverAttackTokenData = _mmTokenDataAttackLocations.FindOrAdd(InIndexOfReservedPoint);
	GiverAttackTokenData.TokensAvailableInSlot = GiverAttackTokenData.TokensAvailableInSlot + InCountTokensTaken;

	if (GiverAttackTokenData.TokensAvailableInSlot > 0)
	{
		GiverAttackTokenData.IsTokenAvailable = true;
	}
}

FVector UAttackTokenGiverComponent::HandleAttackLocationRequest(int32 InIndexOfReservedPoint)
{
	FGiverAttackTokenData GiverAttackTokenData = _mmTokenDataAttackLocations.FindOrAdd(InIndexOfReservedPoint);
	return GiverAttackTokenData.SceneComponentAttackLocation->GetComponentLocation();
}




