// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDClass.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "HUDWidget.h"
#include "Kismet/GameplayStatics.h"

void AHUDClass::BeginPlay()
{
    PlayerController = GetWorld()->GetFirstPlayerController();
    CreateWidgets();
    OpenMainMenu();
}

void AHUDClass::OpenMainMenu()
{
    if (MainMenu)
        MainMenu->AddToViewport();
}

void AHUDClass::CloseMainMenu()
{
    if (MainMenu)
        MainMenu->RemoveFromParent();
}

void AHUDClass::OpenPauseMenu()
{
    if (PauseMenu)
        PauseMenu->AddToViewport();
}

void AHUDClass::ClosePauseMenu()
{
    if (PauseMenu)
        PauseMenu->RemoveFromParent();
}

void AHUDClass::HideHUD()
{
    if (HUD)
        HUD->RemoveFromParent();
}

void AHUDClass::ShowHUD()
{
    if (HUD)
        HUD->AddToViewport();
}

void AHUDClass::CreateWidgets()
{
    if (PlayerController) {
        MainMenu = CreateWidget<UUserWidget>(PlayerController, MainMenuWidgetClass);
        PauseMenu = CreateWidget<UUserWidget>(PlayerController, PauseMenuWidgetClass);
        HUD = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
    }
}

void AHUDClass::UpdateAmmo(float CurrentAmmo, float MaxAmmo, float LeftAmmo)
{
    HUD->UpdateAmmo(CurrentAmmo, MaxAmmo, LeftAmmo);
}

void AHUDClass::UpdateHealth(float CurrentHealth, float MaxHealth)
{
    HUD->UpdateHealth(CurrentHealth, MaxHealth);
}
