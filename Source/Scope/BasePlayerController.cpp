// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"
#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
}
void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent);
    if (EnhancedInput) {
        UE_LOG(LogTemp, Display, TEXT("Binding"));

        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);
        // EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasePlayerController::Sprint);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayerController::EndSprint);
    }
    CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void ABasePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    CurrentCharacter = Cast<ABaseCharacter>(InPawn);
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer())) {
        Subsystem->AddMappingContext(PlayerMappingContext, 0);
        UE_LOG(LogTemp, Display, TEXT("Mapping C"));
    }
    SetupInputComponent();
}
void ABasePlayerController::Move(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Display, TEXT("Move"));

    CurrentCharacter->AddMovementInput(CurrentCharacter->GetActorForwardVector() * Value.Get<FVector2D>().X);
    CurrentCharacter->AddMovementInput(CurrentCharacter->GetActorRightVector() * Value.Get<FVector2D>().Y);
}
void ABasePlayerController::Look(const FInputActionValue& Value)
{
    CurrentCharacter->AddControllerPitchInput(-Value.Get<FVector>().Y);
    CurrentCharacter->AddControllerYawInput(Value.Get<FVector>().X);
}
void ABasePlayerController::Sprint(const FInputActionValue& Value)
{
    CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}
void ABasePlayerController::EndSprint(const FInputActionValue& Value)
{
    CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
