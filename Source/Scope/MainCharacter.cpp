// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
    // SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm Component");
    // SpringArmComponent->SetupAttachment(RootComponent);
}

void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
}
