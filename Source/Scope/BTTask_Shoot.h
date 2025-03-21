// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTTask_Shoot.generated.h"
/**
 *
 */
UCLASS()
class SCOPE_API UBTTask_Shoot : public UBTTaskNode {
    GENERATED_BODY()

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UBTTask_Shoot();
};
