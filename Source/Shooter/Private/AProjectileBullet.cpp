#include "AProjectileBullet.h"
#include "ABulletPool.h"
#include "AEnemyBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick=false;

	CollisionSphere=CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(5.f);
	CollisionSphere->SetCollisionProfileName(FName("Projectile"));
	CollisionSphere->OnComponentHit.AddDynamic(this,&AProjectileBullet::OnHit);
	SetRootComponent(CollisionSphere);

	BulletMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(CollisionSphere);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMesh->CastShadow=false;

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed=3000.f;
	ProjectileMovement->MaxSpeed=3000.f;
	ProjectileMovement->bRotationFollowsVelocity=true;
	ProjectileMovement->bShouldBounce=false;
	ProjectileMovement->ProjectileGravityScale=0.1f;

	Lifetime=3.f;
	CurrentDamage=20.f;
	bBulletActive=false;
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	Deactivate();
}

void AProjectileBullet::Activate(const FVector& Location,const FRotator& Rotation,float Damage,float Speed,AActor* InstigatorActor,ABulletPool* Pool)
{
	OwningPool=Pool;
	CurrentDamage=Damage;

	SetInstigator(Cast<APawn>(InstigatorActor));

	SetActorLocationAndRotation(Location,Rotation);

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	bBulletActive=true;

	ProjectileMovement->SetUpdatedComponent(CollisionSphere);
	ProjectileMovement->InitialSpeed=Speed;
	ProjectileMovement->MaxSpeed=Speed;
	ProjectileMovement->Velocity=Rotation.Vector()*Speed;
	ProjectileMovement->Activate();

	GetWorldTimerManager().SetTimer(
		LifetimeTimer,
		this,
		&AProjectileBullet::OnLifetimeExpired,
		Lifetime,
		false
	);
}

void AProjectileBullet::Deactivate()
{
	bBulletActive=false;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();

	GetWorldTimerManager().ClearTimer(LifetimeTimer);

	SetActorLocation(FVector(0.f,0.f,-10000.f));
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit)
{
	if(!OtherActor||OtherActor==this)return;
	if(OtherActor==GetInstigator())return;

	AEnemyBase* Enemy=Cast<AEnemyBase>(OtherActor);

	if(Enemy)
	{
		Enemy->TakeDamageAmount(CurrentDamage);
	}
	else
	{
		UGameplayStatics::ApplyPointDamage(
			OtherActor,
			CurrentDamage,
			Hit.ImpactNormal,
			Hit,
			GetInstigator()
				? GetInstigator()->GetController()
				: nullptr,
			GetInstigator(),
			nullptr
		);
	}

	if(ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}

	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,Hit.ImpactPoint);
	}

	if(OwningPool)
	{
		OwningPool->ReturnBullet(this);
	}
	else
	{
		Deactivate();
	}
}

void AProjectileBullet::OnLifetimeExpired()
{
	if(OwningPool)
	{
		OwningPool->ReturnBullet(this);
	}
	else
	{
		Deactivate();
	}
}