// Copyright Peasant Pals 2025


#include "Characters/Player/CharacterBasePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Components/BoxComponent.h"
#include "Components/TokenComponents/AttackTokenGiverComponent.h"
#include "Components/SceneComponent.h"


ACharacterBasePlayer::ACharacterBasePlayer()
{
    RocketLauncher = CreateDefaultSubobject<URocketLauncherComponent>(TEXT("RocketLauncherComponent"));
    BoxComponentInteraction = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentInteraction"));
    //SceneComponentEnemyAttackLocationLeft = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationLeft"));
    //SceneComponentEnemyAttackLocationMid = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationMid"));
    //SceneComponentEnemyAttackLocationRight = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentEnemyAttackLocationRight"));
    //SceneComponentPinnerAimLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentPinnerAimLocation"));

    
    DelegateBindingComponentPlayer = CreateDefaultSubobject<UDelegateBindingComponentPlayer>(TEXT("DelegateBindingComponentPlayer"));
    AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));


    AttackTokenGiverableComponent = CreateDefaultSubobject<UAttackTokenGiverComponent>(TEXT("AttackTokenGiverComponent"));
    
    AIPerceptionStimuliSourceComponent->ComponentTags.Add("Player");
    AIPerceptionStimuliSourceComponent->bAutoRegister = true;
    AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
    AIPerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
    
    BoxComponentInteraction->SetupAttachment(GetRootComponent());
    //SceneComponentEnemyAttackLocationLeft->SetupAttachment(GetRootComponent());
    //SceneComponentEnemyAttackLocationMid->SetupAttachment(GetRootComponent());
    //SceneComponentEnemyAttackLocationRight->SetupAttachment(GetRootComponent());

    GameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Player")));
    TeamId = FGenericTeamId(1);

    //_maEnemyAttackLocations = {SceneComponentEnemyAttackLocationLeft, SceneComponentEnemyAttackLocationMid, SceneComponentEnemyAttackLocationRight};
    
    
}

// TArray<FVector> ACharacterBasePlayer::GetEnemyAttackLocations_Implementation()
// {
//     IEnemyDamageable::GetEnemyAttackLocations_Implementation();
//     TArray<FVector> AttackLocations;
//     for (USceneComponent* SceneComponent : _maEnemyAttackLocations)
//     {
//         AttackLocations.Add(SceneComponent->GetComponentLocation());
//     }
//     return AttackLocations;
// }

// TArray<FVector> ACharacterBasePlayer::GetEnemyAttackLocations()
// {
//     TArray<FVector> AttackLocations;
//     // for (USceneComponent* SceneComponent : _maEnemyAttackLocations)
//     // {
//     //     AttackLocations.Add(SceneComponent->GetComponentLocation());
//     // }
//     return AttackLocations;
// }

void ACharacterBasePlayer::PopulateEnemyAttackLocationsArray(USceneComponent* EnemyAttackLocation)
{
    _maEnemyAttackLocations.Add(EnemyAttackLocation);
}

void ACharacterBasePlayer::HandleReturningToken_Implementation(int32 InTokensRequiredForAction, int32 InIndexOfReservedPoint)
{
    if (!AttackTokenGiverableComponent) return;
    AttackTokenGiverableComponent->HandleReturningToken(InTokensRequiredForAction, InIndexOfReservedPoint);
}

bool ACharacterBasePlayer::HandleResevereTokenRequest_Implementation(
    const FRequesterGeneralTokenData& RequesterTokenData, const TArray<int32>& RequestedIndexes,FRequesterReservationData& ReservationData)
{
    if (!AttackTokenGiverableComponent) return false;
    return AttackTokenGiverableComponent->HandleResevereTokenRequest(RequesterTokenData,RequestedIndexes, ReservationData);    
}

FVector ACharacterBasePlayer::HandleAttackLocationRequest_Implementation(int32 InIndexOfReservedPoint)
{
    return AttackTokenGiverableComponent->HandleAttackLocationRequest(InIndexOfReservedPoint);
}

bool ACharacterBasePlayer::GetIsBlastRadiusPowerupEnabled_Implementation()
{
    return _mIsBlastRadiusPowerUpEnabled;
}

void ACharacterBasePlayer::SetIsBlastRadiusPowerupEnabled_Implementation(bool InIsBlastRadiusPowerUpEnabled)
{
    _mIsBlastRadiusPowerUpEnabled = InIsBlastRadiusPowerUpEnabled;
}

void ACharacterBasePlayer::OnOverlapCallBlastRadiusPickupTimer_Implementation()
{
    IPowerupBlastRadiusable::OnOverlapCallBlastRadiusPickupTimer_Implementation();
}

void ACharacterBasePlayer::OnOverlapROFPickupTimer_Implementation()
{
    IPowerupFireRateable::OnOverlapROFPickupTimer_Implementation();
}

void ACharacterBasePlayer::OnGameUnpausedUnPauseBuses_Implementation()
{
}

void ACharacterBasePlayer::OnGamePausedPauseBuses_Implementation()
{
}


// TArray<FVector>& ACharacterBasePlayer::GetEnemyAttackLocations()
// {
//     IEnemyDamageable::GetEnemyAttackLocations();
//     return _maEnemyAttackLocations;
// }
//
// void ACharacterBasePlayer::PopulateEnemyAttackLocationsArray(FVector EnemyAttackLocation)
// {
//     IEnemyDamageable::PopulateEnemyAttackLocationsArray(EnemyAttackLocation);
//     _maEnemyAttackLocations.Add(EnemyAttackLocation);
// }


void ACharacterBasePlayer::OnGameOverStopBuses_Implementation()
{
}

void ACharacterBasePlayer::OnGameOverStopAllExecptMusicBuses_Implementation()
{
}

void ACharacterBasePlayer::BeginPlay()
{
    Super::BeginPlay();
    //bind OnbeginOverlap delegate to custom overlap function.
    BoxComponentInteraction->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACharacterBasePlayer::OnBeginOverlapForInteractableToggleMessageOn);

    
    //Add moppping context
        if (APlayerController* PC = Cast<APlayerController>(Controller)) 
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PC->GetLocalPlayer())) 
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    if (DelegateBindingComponentPlayer)
    {
        DelegateBindingComponentPlayer->GameStateEventManager->OnRequestPlayerHealthDelegate.AddUniqueDynamic(
            this, &ACharacterBasePlayer::OnHealthRequestSendHealth);
    }

    if (HealthComponent)
    {
        HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &ACharacterBasePlayer::OnHealthChangeSendHealth);
    }
    
    OnBeginplayBroadcastActorInitializationDelegates();

    // PopulateEnemyAttackLocationsArray( SceneComponentEnemyAttackLocationLeft);
    // PopulateEnemyAttackLocationsArray(SceneComponentEnemyAttackLocationMid);
    // PopulateEnemyAttackLocationsArray(SceneComponentEnemyAttackLocationRight);
}


void ACharacterBasePlayer::OnBeginplayBroadcastActorInitializationDelegates_Implementation()
{
    IDelegateBindBroadcastable::OnBeginplayBroadcastActorInitializationDelegates_Implementation();

    if (DelegateBindingComponentPlayer)
    {
        DelegateBindingComponentPlayer->GameStateEventManager->OnBeginPlaySendPlayerHealthDelegate.Broadcast(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
    }
    
    
}


void ACharacterBasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Fire NOT USED HERE
        //EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ACharacterBasePlayer::HandleFireStarted);
        //EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &ACharacterBasePlayer::HandleFireStopped);
        
        // Jumping
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBasePlayer::Move);

        // Looking
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBasePlayer::Look);
    }
}

void ACharacterBasePlayer::OnBeginOverlapForInteractableToggleMessageOn(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor->Implements<UInteractable>())
    {
        OverlapActorInteractable = OtherActor;
        FRotator CurrentRotation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetCapsuleComponent()->GetComponentRotation().GetInverse();
        IInteractable::Execute_InteractMessageOn(OtherActor, CurrentRotation);
        // if (UCapsuleComponent* CapsuleComponentOtherActor = OverlapActorInteractable->FindComponentByClass<UCapsuleComponent>())
        // {
        //     FRotator CurrentRotation = CapsuleComponentOtherActor->GetComponentRotation();
        //     IInteractable::Execute_InteractMessageOn(OtherActor, CurrentRotation);
        // }
       
    }
}

void ACharacterBasePlayer::OnEndOverlapForInteractableToggleMessageOff(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->Implements<UInteractable>())
    {
        if (IsValid(OverlapActorInteractable))
        {
            if (IInteractable* Interactable = Cast<IInteractable>(OverlapActorInteractable))
            {
                Interactable->InteractMessageOff();
            }
            OverlapActorInteractable = nullptr;
        }
       
    }
}



void ACharacterBasePlayer::OnHealthChangeSendHealth(float NewHealth, float OldHealth)
{
    DelegateBindingComponentPlayer->GameStateEventManager->
        OnHealthChangePlayerSendHealthDataDelegate.Broadcast(
        NewHealth, OldHealth, HealthComponent->GetMaxHealth());
}

void ACharacterBasePlayer::OnHealthRequestSendHealth()
{
    DelegateBindingComponentPlayer->GameStateEventManager->OnRequestHealthSendHealthDelegate.Broadcast(
        HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());
}

//IEnemey
// void ACharacterBasePlayer::PopulateEnemyAttackLocationsArray(FVector EnemyAttackLocation)
// {
//     _maEnemyAttackLocations.Add(EnemyAttackLocation);
// }
//
// TArray<FVector>& ACharacterBasePlayer::GetEnemyAttackLocations()
// {
//     return _maEnemyAttackLocations;
// }

//TODO replace this with the Array return function GetAttackLocations above
// FVector ACharacterBasePlayer::GetEnemeyAttackLocation()
// {
//     return SceneComponentEnemyAttackLocationMid->GetComponentLocation();
// }

void ACharacterBasePlayer::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add movement 
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void ACharacterBasePlayer::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

