// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "CoreMinimal.h"
#include "SetPlayerIfSeen.generated.h"

/**
 *
 */
UCLASS()
class SCOPE_API USetPlayerIfSeen : public UBTService_BlackboardBase {
    GENERATED_BODY()
public:
    USetPlayerIfSeen();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    TObjectPtr<APawn> Player;
};
