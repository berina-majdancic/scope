// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "BasePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class ABaseCharacter;
struct FInputActionValue;
/**
 *
 */
UCLASS()
class SCOPE_API ABasePlayerController : public APlayerController {
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

public:
    virtual void SetupInputComponent() override;

protected:
    virtual void OnPossess(APawn* InPawn) override;
    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);

    void Sprint(const FInputActionValue& Value);

    void EndSprint(const FInputActionValue& Value);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> JumpAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SprintAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> PlayerMappingContext;

    TObjectPtr<ABaseCharacter> CurrentCharacter;
    UPROPERTY(EditAnywhere)
    float WalkSpeed = 300;
    UPROPERTY(EditAnywhere)
    float RunSpeed = 500;
};
