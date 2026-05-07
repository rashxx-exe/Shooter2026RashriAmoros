#include "AEnemyBase.h"
#include "AEnemyAIController.h"
#include "FPCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick=true;

	MaxHealth=100.f;
	CurrentHealth=MaxHealth;
	AttackDamage=15.f;
	AttackRange=200.f;
	AttackCooldown=1.5f;
	DetectionRange=1500.f;
	LoseTargetRange=2500.f;

	EnemyState=EEnemyState::Idle;
	bIsDead=false;
	CurrentPatrolIndex=0;

	AIControllerClass=AEnemyAIController::StaticClass();
	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth=MaxHealth;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsDead)return;

	APawn* PlayerPawn=UGameplayStatics::GetPlayerPawn(this,0);
	if(!PlayerPawn)return;

	const float DistToPlayer=FVector::Dist(GetActorLocation(),PlayerPawn->GetActorLocation());

	switch(EnemyState)
	{
		case EEnemyState::Idle:
		case EEnemyState::Patrol:
			if(DistToPlayer<=DetectionRange)
			{
				SetTarget(PlayerPawn);
				SetState(EEnemyState::Chasing);
			}
			break;

		case EEnemyState::Alerted:
		case EEnemyState::Chasing:
			if(DistToPlayer>LoseTargetRange)
			{
				SetTarget(nullptr);
				SetState(EEnemyState::Patrol);
			}
			else if(DistToPlayer<=AttackRange)
			{
				SetState(EEnemyState::Attacking);
			}
			break;

		case EEnemyState::Attacking:
			if(DistToPlayer>AttackRange)
			{
				SetState(EEnemyState::Chasing);
			}
			break;

		default:
			break;
	}
}

void AEnemyBase::TakeDamageAmount(float Damage)
{
	if(bIsDead)return;

	CurrentHealth=FMath::Clamp(CurrentHealth-Damage,0.f,MaxHealth);

	if(HitReactMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitReactMontage);
	}

	if(EnemyState==EEnemyState::Idle||EnemyState==EEnemyState::Patrol)
	{
		APawn* Player=UGameplayStatics::GetPlayerPawn(this,0);

		if(Player)
		{
			SetTarget(Player);
			SetState(EEnemyState::Chasing);
		}
	}

	if(CurrentHealth<=0.f)
	{
		Die();
	}
}

void AEnemyBase::Die()
{
	if(bIsDead)return;

	bIsDead=true;

	SetState(EEnemyState::Dead);

	if(AAIController* AIC=Cast<AAIController>(GetController()))
	{
		AIC->BrainComponent->StopLogic(TEXT("Dead"));
		AIC->UnPossess();
	}

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(DeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DeathEffect,
			GetActorLocation()
		);
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	SetLifeSpan(5.f);
}

void AEnemyBase::PerformAttack()
{
	if(bIsDead||!TargetActor)return;

	if(AttackMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}

	const float Dist=FVector::Dist(GetActorLocation(),TargetActor->GetActorLocation());

	if(Dist<=AttackRange)
	{
		AFPCharacter* Player=Cast<AFPCharacter>(TargetActor);

		if(Player)
		{
			Player->TakeDamageAmount(AttackDamage);
		}
		else
		{
			UGameplayStatics::ApplyDamage(TargetActor,AttackDamage,GetController(),this,nullptr);
		}
	}
}

void AEnemyBase::SetState(EEnemyState NewState)
{
	EnemyState=NewState;
}

void AEnemyBase::SetTarget(AActor* NewTarget)
{
	TargetActor=NewTarget;
}

FVector AEnemyBase::GetNextPatrolPoint()
{
	if(PatrolPoints.Num()==0)
	{
		return GetActorLocation();
	}

	AActor* Point=PatrolPoints[CurrentPatrolIndex];
	CurrentPatrolIndex=(CurrentPatrolIndex+1)%PatrolPoints.Num();

	return Point?Point->GetActorLocation():GetActorLocation();
}

bool AEnemyBase::CanAttackTarget() const
{
	if(!TargetActor||bIsDead)return false;

	const float Dist=FVector::Dist(GetActorLocation(),TargetActor->GetActorLocation());
	return Dist<=AttackRange;
}