#include "UBTTask_ChaseTarget.h"
#include "AEnemyBase.h"
#include "AEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChaseTarget::UBTTask_ChaseTarget()
{
	NodeName=TEXT("Chase Target");
	AcceptanceRadius=150.f;
}

EBTNodeResult::Type UBTTask_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
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

	AActor* Target=Cast<AActor>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::TargetActor)
	);

	if(!Target)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->SetState(EEnemyState::Chasing);

	FAIRequestID RequestID=AIC->MoveToActor(Target,AcceptanceRadius);

	return (RequestID!=FAIRequestID::InvalidRequest)
		? EBTNodeResult::Succeeded
		: EBTNodeResult::Failed;
}