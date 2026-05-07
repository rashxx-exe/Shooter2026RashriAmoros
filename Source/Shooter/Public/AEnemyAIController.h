#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AEnemyAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

namespace BBKeys
{
	static const FName TargetActor=TEXT("TargetActor");
	static const FName PatrolLocation=TEXT("PatrolLocation");
	static const FName EnemyState=TEXT("EnemyState");
	static const FName bCanSeeTarget=TEXT("bCanSeeTarget");
}

UCLASS()
class SHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBlackboardComponent> BlackboardComp;

public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintCallable,Category="AI")
	void SetBlackboardTarget(AActor* Target);

	UFUNCTION(BlueprintCallable,Category="AI")
	void SetBlackboardPatrolLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable,Category="AI")
	void SetBlackboardCanSeeTarget(bool bValue);

	FORCEINLINE UBlackboardComponent* GetBlackboard() const {return BlackboardComp;}
	FORCEINLINE UBehaviorTreeComponent* GetBTComponent() const {return BehaviorTreeComp;}
};