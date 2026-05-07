#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AProjectileBullet.generated.h"

class ABulletPool;
class USphereComponent;

UCLASS()
class SHOOTER_API AProjectileBullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BulletMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|Settings", meta = (AllowPrivateAccess = "true"))
	float Lifetime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ImpactEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|State", meta = (AllowPrivateAccess = "true"))
	bool bBulletActive;
	UPROPERTY()
	TObjectPtr<ABulletPool> OwningPool;
	float CurrentDamage;
	FTimerHandle LifetimeTimer;

public:
	AProjectileBullet();

	virtual void BeginPlay() override;

	void Activate(const FVector& Location, const FRotator& Rotation, float Damage, float Speed, AActor* InstigatorActor, ABulletPool* Pool);
	void Deactivate();

	FORCEINLINE bool IsActive() const { return bBulletActive; }

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void OnLifetimeExpired();
};