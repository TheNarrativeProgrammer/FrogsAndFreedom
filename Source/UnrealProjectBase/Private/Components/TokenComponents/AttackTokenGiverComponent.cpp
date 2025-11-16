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

	// Gather components for looping
	TArray<USceneComponent*> FlusherComponents = {
		SceneComponentEnemyAttackLocationLeftFlusher,
		SceneComponentEnemyAttackLocationMidFlusher,
		SceneComponentEnemyAttackLocationRightFlusher
	};

	TArray<USceneComponent*> PinnerComponents = {
		SceneComponentEnemyAttackLocationLeftPinner,
		SceneComponentEnemyAttackLocationMidPinner,
		SceneComponentEnemyAttackLocationRightPinner
	};
	
	SetUpAttachments(0, FlusherComponents,
		_mTotalAttackTokensFlusher, TEXT("_mTotalAttackTokensFlusher"));
	SetUpAttachments(3, PinnerComponents,
		_mTotalAttackTokensPinner, TEXT("_mTotalAttackTokensPinner"));
	
}

void UAttackTokenGiverComponent::SetUpAttachments(
	int32 BaseIndex, const TArray<USceneComponent*>& Components,
	int32 TotalTokens, const FString& Label)
{
	for (USceneComponent* Comp : Components)
	{
		Comp->AttachToComponent(GetOwner()->GetRootComponent(),
								FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	if (TotalTokens <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("DESIGNER %s has zero slots"), *Label);
		for (int32 i = 0; i < 3; ++i)
			AddAttackLocationToMap(
				BaseIndex + i, Components[i], false, 0);
		return;
	}

	if (TotalTokens < 3)
	{
		for (int32 i = 0; i < 3; ++i)
		{
			bool IsTokenAvailable = i < TotalTokens;
			int32 TokensAvailable = IsTokenAvailable ? 1 : 0;
			AddAttackLocationToMap(
				BaseIndex + i, Components[i], IsTokenAvailable, TokensAvailable);
		}
		return;
	}
	
	int32 Remainder = TotalTokens % 3;
	int32 TokensPerSlot = TotalTokens / 3;

	for (int32 i = 0; i < 3; ++i)
	{
		int32 TokensAvailable = TokensPerSlot + (i < Remainder ? 1 : 0);
		AddAttackLocationToMap(
			BaseIndex + i, Components[i], true, TokensAvailable);
	}
}

void UAttackTokenGiverComponent::AddAttackLocationToMap(
	int32 Key, USceneComponent* SceneComponentAttackLocation,
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
const TArray<int32> RequestedIndexes,FRequesterReservationData& RequesterReservationData)
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




