#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "FPCharacter.generated.h"

class UWeaponBase;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SHOOTER_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FP|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FPMesh;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FP|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FPCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Weapons", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWeaponBase>> WeaponClasses;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Weapons", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWeaponBase> CurrentWeapon;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Weapons", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UWeaponBase>> Weapons;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Weapons", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWeaponIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FP|Stats", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;	
	
public:
	AFPCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "FP|Combat")
	void TakeDamageAmount(float Damage);

	UFUNCTION(BlueprintCallable, Category = "FP|Combat")
	void Die();

	void DoMove(float Forward, float Right);
	void DoLook(float Yaw, float Pitch);
	
protected:
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJumpStarted(const FInputActionValue& Value);
	void OnJumpCompleted(const FInputActionValue& Value);
	void OnFireStarted(const FInputActionValue& Value);
	void OnFireCompleted(const FInputActionValue& Value);
	void OnAimStarted(const FInputActionValue& Value);
	void OnAimCompleted(const FInputActionValue& Value);
	void OnReload(const FInputActionValue& Value);
	void OnSwitchWeapon(const FInputActionValue& Value);
	void OnSprintStarted(const FInputActionValue& Value);
	void OnSprintCompleted(const FInputActionValue& Value);

	void InitWeapons();
	void EquipWeapons(int32 Index);
	
public:
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const { return FPMesh; }
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const { return FPCamera; }
	FORCEINLINE UWeaponBase* GetCurrentWeapon() const { return CurrentWeapon ;}
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
	FORCEINLINE bool GetIsDead() const { return bIsDead; }

};
