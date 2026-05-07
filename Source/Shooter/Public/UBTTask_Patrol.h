#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_Patrol.generated.h"

UCLASS()
class SHOOTER_API UBTTask_Patrol : public UBTTaskNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float AcceptanceRadius;

public:
    UBTTask_Patrol();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};