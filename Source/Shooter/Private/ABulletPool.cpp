#include "ABulletPool.h"
#include "AProjectileBullet.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ABulletPool::ABulletPool()
{
	PrimaryActorTick.bCanEverTick=false;
	PoolSize=30;
	bCanGrow=true;
}

void ABulletPool::BeginPlay()
{
	Super::BeginPlay();
}

ABulletPool* ABulletPool::GetPool(UWorld* World)
{
	if(!World)return nullptr;

	AActor* Found=UGameplayStatics::GetActorOfClass(World,ABulletPool::StaticClass());
	return Cast<ABulletPool>(Found);
}

void ABulletPool::SpawnBullet(TSubclassOf<AActor> BulletClass,const FVector& Location,const FRotator& Rotation,float Damage,float Speed,AActor* InInstigator)
{
	if(!BulletClass)return;

	AProjectileBullet* Bullet=GetOrCreate(BulletClass);
	if(!Bullet)return;

	Bullet->Activate(Location,Rotation,Damage,Speed,InInstigator,this);
}

void ABulletPool::ReturnBullet(AProjectileBullet* Bullet)
{
	if(!Bullet)return;

	Bullet->Deactivate();
}

void ABulletPool::PrewarmClass(TSubclassOf<AActor> BulletClass,int32 Count)
{
	UWorld* World=GetWorld();
	if(!World||!BulletClass)return;

	TArray<AProjectileBullet*>& BulletArray=PoolMap.FindOrAdd(BulletClass);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for(int32 i=0;i<Count;++i)
	{
		AProjectileBullet* NewBullet=World->SpawnActor<AProjectileBullet>(
			BulletClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);

		if(NewBullet)
		{
			NewBullet->Deactivate();
			BulletArray.Add(NewBullet);
		}
	}
}

AProjectileBullet* ABulletPool::GetOrCreate(TSubclassOf<AActor> BulletClass)
{
	TArray<AProjectileBullet*>* BulletArrayPtr=PoolMap.Find(BulletClass);

	if(!BulletArrayPtr)
	{
		PrewarmClass(BulletClass,PoolSize);
		BulletArrayPtr=PoolMap.Find(BulletClass);
	}

	if(!BulletArrayPtr)return nullptr;

	for(AProjectileBullet* Bullet:*BulletArrayPtr)
	{
		if(Bullet&&!Bullet->IsActive())
			return Bullet;
	}

	if(bCanGrow)
	{
		UWorld* World=GetWorld();
		if(!World)return nullptr;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProjectileBullet* NewBullet=World->SpawnActor<AProjectileBullet>(
			BulletClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);

		if(NewBullet)
		{
			NewBullet->Deactivate();
			BulletArrayPtr->Add(NewBullet);
			return NewBullet;
		}
	}

	return nullptr;
}