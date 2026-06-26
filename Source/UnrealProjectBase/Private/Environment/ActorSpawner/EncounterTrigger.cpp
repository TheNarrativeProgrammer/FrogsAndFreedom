// Copyright Peasant Pals 2025


#include "Environment/ActorSpawner/EncounterTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Misc/DataValidation.h"
#include "GameFramework/GameMode.h"
#include "Characters/Player/CharacterBasePlayer.h"
#include "Components/EncounterManagerComponent.h"
#include "Interfaces/ObjectiveSpawnUntilCompleteable.h"
#include "Objectives/ObjectiveActorBase.h"


// Sets default values
AEncounterTrigger::AEncounterTrigger()
{
	DelegateBindComponentEncountrTrg = CreateDefaultSubobject<UDelegateBindComponentEncountrTrg>(TEXT("DelegateBindgingComponent"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");

}


void AEncounterTrigger::OnEncounterEnd_Implementation(
	UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	if (_mCountCompletedObjectives >= _mCountTotalObjectives)
	{
		if (OtherActor->Implements<UPlayerCharacterable>())
		{
			if (TriggerBox)
			{
				TriggerBox->OnComponentEndOverlap.RemoveDynamic(
				this, &AEncounterTrigger::OnEncounterEnd);
			}
			UnregisterFromEncounterManager();
		}
	}
}

void AEncounterTrigger::StartObjectives()
{
	for (AObjectiveActorBase* Objective  : ObjectiveActors)
	{
		if (Objective != nullptr)
		{
			if (Objective->bStartWithEncounter)
			{
				Objective->StartObjective();
			}
		}
	}
}

void AEncounterTrigger::SetCountOfTotalObjectives()
{
	if (ObjectiveActors.IsEmpty() == false)
	{
		for (AObjectiveActorBase* ObjectiveActor : ObjectiveActors)
		{
			if (!ObjectiveActor)
			{
				continue;
			}
			if (ObjectiveActor->Implements<UObjectiveable>())
			{
				if (IObjectiveable::Execute_GetIsMustCompleteToOpenDoor(
					ObjectiveActor) == true)
				{
					_mCountTotalObjectivesInfluencingDoorOpening++;
				}
				if (IObjectiveable::Execute_GetIsMustCompleteToStopSpawn(
					ObjectiveActor) == true)
				{
					_mCountTotalObjectivesInfluencingSpawnStopping++;
				}
			}
		}
		_mCountTotalObjectives = ObjectiveActors.Num();
	}
}


void AEncounterTrigger::IfAllObjectivesCompeleteThenBroadcast()
{
	SetObjectiveCountsToZero();
	UpdateCountOfCompletedObjectives();
	BroadcastObjectiveCompletionGameEvents();
}

void AEncounterTrigger::UpdateCountOfCompletedObjectives()
{
	for (AObjectiveActorBase* ObjectiveActor : ObjectiveActors)
	{
		if (ObjectiveActor->Implements<UObjectiveable>())
		{
			if (ObjectiveActor->IsObjectiveCompleted())
			{
				_mCountCompletedObjectives++;
				if (IObjectiveable::Execute_GetIsMustCompleteToOpenDoor(ObjectiveActor))
				{
					_mCountCompletedObjectivesInfluencingDoorOpening++;
				}
				if (IObjectiveable::Execute_GetIsMustCompleteToStopSpawn(ObjectiveActor))
				{
					_mCountCompletedObjectivesInfluencingSpawnStopping++;
				}
			}
		}
	}
}

void AEncounterTrigger::BroadcastObjectiveCompletionGameEvents()
{
	if (_mCountCompletedObjectives >= _mCountTotalObjectives)
	{
		DelegateBindComponentEncountrTrg->GameStateEventManager->
			OnAllObectivesCompleteDelegate.Broadcast();
	}
	//Objectives influencing spawn stopping complete
	if (_mCountTotalObjectivesInfluencingSpawnStopping > 0
		&& _mCountCompletedObjectivesInfluencingSpawnStopping >=
			_mCountTotalObjectivesInfluencingSpawnStopping
		&& _mHasSpawnStopped == false)
	{
		_mHasSpawnStopped = true;
		DelegateBindComponentEncountrTrg->GameStateEventManager->
			OnAllObjectiveInfluencingSpawningCompleteDelegate.Broadcast();
	}
	//Objectives influencing door complete
	if (_mCountCompletedObjectivesInfluencingDoorOpening >=
			_mCountTotalObjectivesInfluencingDoorOpening
		&& _mIsDoorOpened == false)
	{
		_mIsDoorOpened = true;
		DelegateBindComponentEncountrTrg->GameStateEventManager->
			OnAllObjectiveInfluencingDoorOpeningCompleteDelegate.Broadcast();
	}
}

void AEncounterTrigger::SetObjectiveCountsToZero()
{
	_mCountCompletedObjectives = 0;
	_mCountCompletedObjectivesInfluencingDoorOpening = 0;
	_mCountCompletedObjectivesInfluencingSpawnStopping = 0;
}


void AEncounterTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic(
			this, &AEncounterTrigger::OnEncounterBegin);
		TriggerBox->OnComponentEndOverlap.AddUniqueDynamic(
			this, &AEncounterTrigger::OnEncounterEnd);
	}

	SetObjectiveCountsToZero();
	SetCountOfTotalObjectives();
	
	if (TriggerBox)
	{
		if (ACharacter* PlayerCharacter = UGameplayStatics::
			GetPlayerCharacter(GetWorld(), 0))
		{
			if (ACharacterBasePlayer* CharacterBasePlayer = Cast<
				ACharacterBasePlayer>(PlayerCharacter))
			{
				if (TriggerBox->IsOverlappingActor(CharacterBasePlayer))
				{
					TriggerBox->OnComponentBeginOverlap.RemoveDynamic(
						this, &AEncounterTrigger::OnEncounterBegin);
					RegisterWithEncounterManager();
				}
			}
		}
	}
	
}

void AEncounterTrigger::OnEncounterBegin_Implementation(
	UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep,const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	if (OtherActor->Implements<UPlayerCharacterable>())
	{
		if (TriggerBox)
		{
			TriggerBox->OnComponentBeginOverlap.RemoveDynamic(
			this, &AEncounterTrigger::OnEncounterBegin);
		}
		RegisterWithEncounterManager();
	}
}

void AEncounterTrigger::RegisterWithEncounterManager()
{
	if (AGameModeBase* GameMode  =
		GetWorld()->GetAuthGameMode())
	{
		if (UEncounterManagerComponent* EncounterManager =
			GameMode->FindComponentByClass<
			UEncounterManagerComponent>())
		{
			EncounterManager->RegisterEncounter(this);
		}
	}
}

void AEncounterTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (TriggerBox)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (TriggerBox->IsOverlappingActor(PlayerCharacter))
		{
			RegisterWithEncounterManager();
		}
	}
}

#if WITH_EDITOR
EDataValidationResult AEncounterTrigger::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (Result == EDataValidationResult::Invalid)
	{
		return  Result;
	}
	if (!EncounterData)
	{
		FText ErrorMsg = FText::FromString("No Encounter Data Is Set");
		Context.AddError(ErrorMsg);
		return EDataValidationResult::Invalid;
	}
	return EDataValidationResult::Valid;
}
#endif



void AEncounterTrigger::UnregisterFromEncounterManager()
{
	
	if (AGameModeBase* GameMode  = GetWorld()->GetAuthGameMode())
	{
		if (UEncounterManagerComponent* EncounterManager = GameMode->FindComponentByClass<UEncounterManagerComponent>())
		{
			EncounterManager->UnregisterEncounter(this);
			EncounterManager->GameStateEventManager->OnLevelLoadResetDataDelegate.Broadcast();
		}
	}
}

void AEncounterTrigger::AutoCompleteObjectives()
{
	for (AObjectiveActorBase* ObjectiveActorBase : ObjectiveActors)
	{
		if (!ObjectiveActorBase)
		{
			continue;
		}
		ObjectiveActorBase->SetIsAutoCompleteObjective(true);
	}
}

// Called every frame
void AEncounterTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEncounterTrigger::OnSkipLevelUnregisterFromEncounterManager()
{
	AutoCompleteObjectives();
	DelegateBindComponentEncountrTrg->GameStateEventManager->OnAllObectivesCompleteDelegate.Broadcast();
	UnregisterFromEncounterManager();
}

