#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AEnemyBase.generated.h"

class UBehaviorTree;
class UAIPerceptionComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EEnemyState:uint8
{
	Idle UMETA(DisplayName="Idle"),
	Patrol UMETA(DisplayName="Patrol"),
	Alerted UMETA(DisplayName="Alerted"),
	Chasing UMETA(DisplayName="Chasing"),
	Attacking UMETA(DisplayName="Attacking"),
	Dead UMETA(DisplayName="Dead")
};

UCLASS()
class SHOOTER_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|AI",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float AttackRange;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float AttackCooldown;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float DetectionRange;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|Stats",meta=(AllowPrivateAccess="true"))
	float LoseTargetRange;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly,Category="Enemy|Patrol",meta=(AllowPrivateAccess="true"))
	TArray<AActor*> PatrolPoints;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Enemy|Patrol",meta=(AllowPrivateAccess="true"))
	int32 CurrentPatrolIndex;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Enemy|State",meta=(AllowPrivateAccess="true"))
	EEnemyState EnemyState;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Enemy|State",meta=(AllowPrivateAccess="true"))
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Enemy|State",meta=(AllowPrivateAccess="true"))
	bool bIsDead;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|VFX",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|VFX",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Enemy|VFX",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UParticleSystem> DeathEffect;

	FTimerHandle AttackTimerHandle;

public:
	AEnemyBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category="Enemy|Combat")
	void TakeDamageAmount(float Damage);

	UFUNCTION(BlueprintCallable,Category="Enemy|Combat")
	void Die();

	UFUNCTION(BlueprintCallable,Category="Enemy|Combat")
	void PerformAttack();

	UFUNCTION(BlueprintCallable,Category="Enemy|AI")
	void SetState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable,Category="Enemy|AI")
	void SetTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable,Category="Enemy|AI")
	FVector GetNextPatrolPoint();

	UFUNCTION(BlueprintCallable,Category="Enemy|AI")
	bool CanAttackTarget() const;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	FORCEINLINE EEnemyState GetEnemyState() const {return EnemyState;}
	FORCEINLINE AActor* GetTarget() const {return TargetActor;}
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}
	FORCEINLINE bool GetIsDead() const {return bIsDead;}
	FORCEINLINE float GetDetectionRange() const {return DetectionRange;}
	FORCEINLINE float GetLoseTargetRange() const {return LoseTargetRange;}
	FORCEINLINE float GetAttackRange() const {return AttackRange;}
};