// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDClass.generated.h"
class ACharacter;
class UHUDWidget;
/**
 *
 */
UCLASS()
class SCOPE_API AHUDClass : public AHUD {
    GENERATED_BODY()
public:
    virtual void BeginPlay() override;
    void OpenMainMenu();
    void CloseMainMenu();
    void OpenPauseMenu();
    void ClosePauseMenu();
    void HideHUD();
    void ShowHUD();
    void CreateWidgets();
    void UpdateAmmo(float CurrentAmmo, float MaxAmmo, float LeftAmmo);
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UHUDWidget> HUDWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UUserWidget> MainMenu;
    UPROPERTY()
    TObjectPtr<UUserWidget> PauseMenu;
    UPROPERTY()
    TObjectPtr<UHUDWidget> HUD;
    UPROPERTY()

    TObjectPtr<APlayerController> PlayerController;

    bool b_IsPlaying = false;
};
