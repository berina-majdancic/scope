// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "HUDWidget.generated.h"

/**
 *
 */
UCLASS()
class SCOPE_API UHUDWidget : public UUserWidget {
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateHealth(float CurrentHealth, float MaxHealth);
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateAmmo(float CurrentAmmo, float MaxAmmo, float LeftAmmo);
};
