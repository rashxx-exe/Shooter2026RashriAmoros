#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AShooterGameMode.generated.h"

class ABulletPool;

UCLASS()
class SHOOTER_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode|Pool", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABulletPool> BulletPoolClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode|Pool", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABulletPool> BulletPoolInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode|Score", meta = (AllowPrivateAccess = "true"))
	int32 Score;

public:
	AShooterGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void OnPlayerDied();

	FORCEINLINE int32 GetScore() const { return Score; }
	FORCEINLINE ABulletPool* GetBulletPool() const { return BulletPoolInstance; }
};