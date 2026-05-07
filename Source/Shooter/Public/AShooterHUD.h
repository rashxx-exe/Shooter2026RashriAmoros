#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AShooterHUD.generated.h"

class UUserWidget;

UCLASS()
class SHOOTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PauseMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> DeathScreenClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|State", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> MainHUDWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|State", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> PauseMenuWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|State", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> DeathScreenWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD|State", meta = (AllowPrivateAccess = "true"))
	bool bIsPaused;

public:
	AShooterHUD();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowMainHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowDeathScreen();

	FORCEINLINE bool IsPaused() const { return bIsPaused; }
};