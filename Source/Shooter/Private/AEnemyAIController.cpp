#include "AEnemyAIController.h"
#include "AEnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComp=CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp=CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyBase* Enemy=Cast<AEnemyBase>(InPawn);
	if(!Enemy)return;

	UBehaviorTree* BT=Enemy->GetBehaviorTree();
	if(!BT)return;

	UBlackboardComponent* BBComp=BlackboardComp.Get();

	if(UseBlackboard(BT->BlackboardAsset,BBComp))
	{
		BlackboardComp=BBComp;
		RunBehaviorTree(BT);
	}
}

void AEnemyAIController::OnUnPossess()
{
	BehaviorTreeComp->StopTree();
	Super::OnUnPossess();
}

void AEnemyAIController::SetBlackboardTarget(AActor* Target)
{
	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(BBKeys::TargetActor,Target);
	}
}

void AEnemyAIController::SetBlackboardPatrolLocation(const FVector& Location)
{
	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(BBKeys::PatrolLocation,Location);
	}
}

void AEnemyAIController::SetBlackboardCanSeeTarget(bool bValue)
{
	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(BBKeys::bCanSeeTarget,bValue);
	}
}