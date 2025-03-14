// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer())) {
            Subsystem->AddMappingContext(PlayerMappingContext, 0);
        }
    }
}
void ABasePlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInput) {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasePlayerController::Sprint);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayerController::EndSprint);
    }
}

void ABasePlayerController::Move(const FInputActionValue& Value)
{
    AddMovementInput(GetActorForwardVector() * Value.Get<FVector2D>().X);
    AddMovementInput(GetActorRightVector() * Value.Get<FVector2D>().Y);
}

void ABasePlayerController::Look(const FInputActionValue& Value)
{
    AddControllerPitchInput(-Value.Get<FVector>().Y);
    AddControllerYawInput(Value.Get<FVector>().X);
}

void ABasePlayerController::Sprint(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
}
void ABasePlayerController::EndSprint(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed /= 2.0f;
}
