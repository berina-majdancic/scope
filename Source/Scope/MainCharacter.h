// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "CoreMinimal.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 *
 */
UCLASS()
class SCOPE_API AMainCharacter : public ABaseCharacter {
    GENERATED_BODY()
public:
    AMainCharacter();
    virtual void BeginPlay() override;

protected:
    // UPROPERTY(VisibleAnywhere, Category = "Character", meta = (AllowPrivateAccess = "true"))
    // TObjectPtr<USpringArmComponent> SpringArmComponent;
};
