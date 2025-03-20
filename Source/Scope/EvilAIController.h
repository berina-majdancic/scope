// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree\BehaviorTree.h"
#include "CoreMinimal.h"
#include "EvilAIController.generated.h"

/**
 *
 */
UCLASS()
class SCOPE_API AEvilAIController : public AAIController {
    GENERATED_BODY()
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UBehaviorTree> EvilBT;
};
