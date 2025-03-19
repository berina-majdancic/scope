// Fill out your copyright notice in the Description page of Project Settings.

#include "EvilAIController.h"

void AEvilAIController::BeginPlay()
{
    Super::BeginPlay();
    if (EvilBT) {
        RunBehaviorTree(EvilBT);
    }
}
