// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"
#include "BaseCharacter.h"

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TObjectPtr<ABaseCharacter> AIPlayer = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AIPlayer) {
        AIPlayer->Shoot();
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = "Shoot";
}
