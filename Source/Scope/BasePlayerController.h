// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#

#include "BasePlayerController.generated.h"
/**
 *
 */
UCLASS()
class SCOPE_API ABasePlayerController : public APlayerController {
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);

    void Sprint(const FInputActionValue& Value);

    void EndSprint(const FInputActionValue& Value);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    UInputAction* MoveAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    UInputAction* LookAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    UInputAction* JumpAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    UInputAction* SprintAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    UInputMappingContext* PlayerMappingContext;
};
