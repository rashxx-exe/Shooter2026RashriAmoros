#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UWeaponBase.generated.h"

class AFPCharacter;
class ABulletPool;
class UAnimMontage;
class UParticleSystem;
class USoundBase;

UENUM(BlueprintType)
enum class EWeaponFireMode  : uint8
{
	SemiAuto UMETA(DisplayName = "SemiAuto"),
	FullAuto UMETA(DisplayName = "FullAuto"),
	Burst UMETA(DisplayName = "Burst"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Hitscan UMETA(DisplayName = "Hitscan"),
	Projectile UMETA(DisplayName = "Projectile"),
};

UCLASS(Blueprintable, BlueprintType)
class SHOOTER_API UWeaponBase : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info", meta = (AllowPrivateAccess = "true"))
	FName WeaponName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	EWeaponFireMode FireMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	float Range;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	float SpreadAngle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Settings", meta = (AllowPrivateAccess = "true"))
	float AimSpreadModifier;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo",meta = (AllowPrivateAccess = "true"))
	int32 MagSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo", meta = (AllowPrivateAccess = "true"))
	int32 ReserveAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo", meta = (AllowPrivateAccess = "true"))
	float ReloadTime;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile", meta = (AllowPrivateAccess = "true", EditCondition = "WeaponType == EWeaponType::Projectile"))
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile", meta = (AllowPrivateAccess = "true", EditCondition = "WeaponType == EWeaponType::Projectile"))
	float ProjectileSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FireMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> MuzzleFlash;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ReloadSound;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State", meta = (AllowPrivateAccess = "true"))
	bool bIsFiring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State", meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
 
	UPROPERTY()
	TObjectPtr<AFPCharacter> OwnerCharacter;
 
	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

public:
	UWeaponBase();

	void InitWeapon(AFPCharacter* Owner);
	void OnEquip();
	void OnUnequip();

	void StartFire();
	void StopFire();
	void Reload();
	void StartAim();
	void StopAim();

	FORCEINLINE FName GetWeaponName() const { return WeaponName; }
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE int32 GetReserveAmmo() const { return ReserveAmmo; }
	FORCEINLINE bool GetIsFiring() const { return bIsFiring; }
	FORCEINLINE bool GetIsReloading() const { return bIsReloading; }
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }

private:
	void FireOnce();
	void FireHitscan();
	void FireProjectile();

	void ApplyHitDamage(AActor* HitActor, const FVector& HitLocation, const FVector& HitNormal, UPhysicalMaterial* PhysMat);

	void FinishReload();
	
	FVector GetFireDirection() const;
	FVector GetMuzzleLocation() const;
};
