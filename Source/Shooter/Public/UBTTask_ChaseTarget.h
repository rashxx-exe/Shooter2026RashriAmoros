#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_ChaseTarget.generated.h"

UCLASS()
class SHOOTER_API UBTTask_ChaseTarget : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Chase")
	float AcceptanceRadius;

public:
	UBTTask_ChaseTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};