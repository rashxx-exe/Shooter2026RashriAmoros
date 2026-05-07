#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABulletPool.generated.h"

class AProjectileBullet;

UCLASS()
class SHOOTER_API ABulletPool : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Pool|Settings",meta=(AllowPrivateAccess="true"))
	int32 PoolSize;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Pool|Settings",meta=(AllowPrivateAccess="true"))
	bool bCanGrow;

	TMap<TSubclassOf<AActor>,TArray<AProjectileBullet*>> PoolMap;

public:
	ABulletPool();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Category="Pool")
	static ABulletPool* GetPool(UWorld* World);

	UFUNCTION(BlueprintCallable,Category="Pool")
	void SpawnBullet(TSubclassOf<AActor> BulletClass,const FVector& Location,const FRotator& Rotation,float Damage,float Speed,AActor* InInstigator);

	void ReturnBullet(AProjectileBullet* Bullet);

private:
	void PrewarmClass(TSubclassOf<AActor> BulletClass,int32 Count);
	AProjectileBullet* GetOrCreate(TSubclassOf<AActor> BulletClass);
};