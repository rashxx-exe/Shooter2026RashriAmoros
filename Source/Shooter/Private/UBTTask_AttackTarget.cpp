#include "UBTTask_AttackTarget.h"
#include "AEnemyBase.h"
#include "AEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName=TEXT("Attack Target");
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
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

	if(!Enemy->CanAttackTarget())
	{
		return EBTNodeResult::Failed;
	}

	Enemy->SetState(EEnemyState::Attacking);
	Enemy->PerformAttack();

	return EBTNodeResult::Succeeded;
}