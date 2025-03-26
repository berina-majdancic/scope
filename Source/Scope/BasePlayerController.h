// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "BasePlayerController.generated.h"
class UInputAction;
class UInputMappingContext;
class ABaseCharacter;
class UUserWidget;
struct FInputActionValue;
class AHUDClass;
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

    UFUNCTION(BlueprintPure)
    bool IsCrouching() const;
    void UpdateAmmo(float CurrentAmmo, float MaxAmmo, float LeftAmmo);
    void UpdateHealth(float CurrentHealth, float MaxHealth);

protected:
    virtual void OnPossess(APawn* InPawn) override;
    void Move(const FInputActionValue& Value);
    void Shoot(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Walk(const FInputActionValue& Value);
    void Sprint(const FInputActionValue& Value);
    void Jump(const FInputActionValue& Value);
    void Crouch(const FInputActionValue& Value);
    void Reload(const FInputActionValue& Value);
    void PauseMenuSwitch(const FInputActionValue& Value);
    void MainMenuDisplay();
    AActor* FindMainMenuCamera();

    UFUNCTION(BlueprintCallable)
    void OnPlayGameClicked();
    UFUNCTION(BlueprintCallable)
    void OnQuitGameClicked();
    UFUNCTION(BlueprintCallable)
    void OnResumeClicked();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> ShootAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> JumpAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> WalkAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> CrouchAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> ReloadAction;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> PlayerMappingContext;
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MainMenuAction;

    UPROPERTY()
    TObjectPtr<ABaseCharacter> CurrentCharacter;
    UPROPERTY()
    TObjectPtr<AActor> MainMenuCamera;
    UPROPERTY()
    TObjectPtr<AHUDClass> HUD;

    UPROPERTY(EditAnywhere)
    float WalkSpeed = 200;
    UPROPERTY(EditAnywhere)
    float CrouchSpeed = 200;
    UPROPERTY(EditAnywhere)
    float RunSpeed = 500;
    UPROPERTY(EditAnywhere)
    float SensitivityY = 0.3;
    UPROPERTY(EditAnywhere)
    float SensitivityX = 0.2;
    bool bIsCrouching = false;
    bool bInMainMenu = false;
};
