// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm Component");
    SpringArmComponent->SetupAttachment(RootComponent);
    CameraComponent->SetupAttachment(SpringArmComponent);
}
