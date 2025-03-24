// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "BaseCharacter.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/CinematicCamera/Public/CineCameraActor.h"
#include <EngineUtils.h>

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
    MainMenuStartDisplay();
}
void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent);
    if (EnhancedInput) {

        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
        EnhancedInput->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Shoot);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerController::Jump);
        EnhancedInput->BindAction(WalkAction, ETriggerEvent::Started, this, &ABasePlayerController::Walk);
        EnhancedInput->BindAction(WalkAction, ETriggerEvent::Completed, this, &ABasePlayerController::Sprint);
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABasePlayerController::Crouch);
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABasePlayerController::Sprint);
        EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Completed, this, &ABasePlayerController::Reload);
        EnhancedInput->BindAction(MainMenuAction, ETriggerEvent::Started, this, &ABasePlayerController::MainMenuGameplayDisplay);
    }
}
bool ABasePlayerController::IsCrouching() const
{
    return bIsCrouching;
}
void ABasePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    CurrentCharacter = Cast<ABaseCharacter>(InPawn);
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer())) {
        Subsystem->AddMappingContext(PlayerMappingContext, 0);
    }
    SetupInputComponent();
    CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}
void ABasePlayerController::Move(const FInputActionValue& Value)
{
    CurrentCharacter->AddMovementInput(CurrentCharacter->GetActorForwardVector() * Value.Get<FVector2D>().X);
    CurrentCharacter->AddMovementInput(CurrentCharacter->GetActorRightVector() * Value.Get<FVector2D>().Y);
}
void ABasePlayerController::Shoot(const FInputActionValue& Value)
{
    CurrentCharacter->Shoot();
}
void ABasePlayerController::Look(const FInputActionValue& Value)
{
    CurrentCharacter->AddControllerPitchInput(-Value.Get<FVector>().Y * SensitivityY);
    CurrentCharacter->AddControllerYawInput(Value.Get<FVector>().X * SensitivityX);
}
void ABasePlayerController::Walk(const FInputActionValue& Value)
{
    CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void ABasePlayerController::Sprint(const FInputActionValue& Value)
{
    if (!bIsCrouching)
        CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ABasePlayerController::Jump(const FInputActionValue& Value)
{
    CurrentCharacter->Jump();
}

void ABasePlayerController::Crouch(const FInputActionValue& Value)
{
    CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
    bIsCrouching = !bIsCrouching;
}

void ABasePlayerController::Reload(const FInputActionValue& Value)
{
    CurrentCharacter->Reload();
}

void ABasePlayerController::MainMenuGameplayDisplay(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Display, TEXT("In main menu "));
    if (!bInMainMenu) {
        UE_LOG(LogTemp, Display, TEXT("In main menu "));
        UE_LOG(LogTemp, Display, TEXT("In main false "));

        if (HUD)
            HUD->RemoveFromParent();

        MainMenu = CreateWidget(this, GameplayMenuWidgetClass);
        if (MainMenu && MainMenuCamera) {
            UE_LOG(LogTemp, Display, TEXT("Posses cam"));
            SetViewTargetWithBlend(MainMenuCamera);
            FInputModeUIOnly UIOnly;
            SetInputMode(UIOnly);
            MainMenu->AddToViewport();
            bInMainMenu = true;
        }
    }
}
void ABasePlayerController::MainMenuStartDisplay()
{
    MainMenu = CreateWidget(this, StartMenuWidgetClass);
    MainMenuCamera = FindMainMenuCamera();
    if (MainMenu && MainMenuCamera) {
        SetViewTargetWithBlend(MainMenuCamera);
        FInputModeUIOnly UIOnly;
        SetInputMode(UIOnly);
        if (MainMenu)
            MainMenu->AddToViewport();
    }
}

void ABasePlayerController::OnPlayGameClicked()
{
    bInMainMenu = false;
    FInputModeGameOnly GameOnly;
    SetInputMode(GameOnly);
    Possess(CurrentCharacter);
    if (MainMenu)
        MainMenu->RemoveFromParent();
    HUD = CreateWidget(this, HUDWidgetClass);
    HUD->AddToViewport();
}

void ABasePlayerController::OnQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, 0);
}

void ABasePlayerController::OnResumeClicked()
{
    HUD = CreateWidget(this, HUDWidgetClass);
    HUD->AddToViewport();
    bInMainMenu = false;
    FInputModeGameOnly GameOnly;
    SetInputMode(GameOnly);
    Possess(CurrentCharacter);
    if (MainMenu)
        MainMenu->RemoveFromParent();
}

AActor* ABasePlayerController::FindMainMenuCamera()
{
    for (ACineCameraActor* Camera : TActorRange<ACineCameraActor>(GetWorld())) {
        if (Camera->ActorHasTag("MainMenuCamera"))
            return Camera;
    }
    return nullptr;
}
