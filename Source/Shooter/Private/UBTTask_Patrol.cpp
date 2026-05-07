#include "UBTTask_Patrol.h"
#include "AEnemyBase.h"
#include "AEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName=TEXT("Patrol");
	AcceptanceRadius=100.f;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
{
	AEnemyAIController* AIC=Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if(!AIC)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyBase* Enemy=Cast<AEnemyBase>(AIC->GetPawn());

	if(!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	const FVector NextPoint=Enemy->GetNextPatrolPoint();

	AIC->SetBlackboardPatrolLocation(NextPoint);

	Enemy->SetState(EEnemyState::Patrol);

	FAIRequestID RequestID=AIC->MoveToLocation(NextPoint,AcceptanceRadius);

	return (RequestID!=FAIRequestID::InvalidRequest)
		? EBTNodeResult::Succeeded
		: EBTNodeResult::Failed;
}