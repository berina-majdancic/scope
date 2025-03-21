// Fill out your copyright notice in the Description page of Project Settings.

#include "SetPlayerIfSeen.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

USetPlayerIfSeen::USetPlayerIfSeen()
{
    NodeName = "Set Player If Seen";
}

void USetPlayerIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Player) {
        if (OwnerComp.GetAIOwner()->LineOfSightTo(Player)) {
            OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);
        } else {
            OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
}
