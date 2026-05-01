#include "UWeaponBase.h"

#include "FPCharacter.h"
#include "FPEnemyBase.h"
#include "ABulletPool.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"

UWeaponBase::UWeaponBase()
{
	WeaponName = TEXT("BaseWeapon");
	FireMode = EWeaponFireMode::SemiAuto;
	WeaponType = EWeaponType::Hitscan;
	FireRate = 10.f;
	Damage = 20.f;
	Range = 5000.f;
	SpreadAngle = 1.5f;
	AimSpreadModifier = 0.25f;
	MagSize = 30;
	CurrentAmmo = MagSize;
	ReserveAmmo = 90;
	ReloadTime = 2.0f;
	ProjectileSpeed = 3000.f;

	bIsFiring = false;
	bIsReloading = false;
	bIsAiming = false;
}

void UWeaponBase::InitWeapon(AFPCharacter* Owner)
{
	OwnerCharacter = Owner;
	CurrentAmmo = MagSize;
}

void UWeaponBase::OnEquip()
{

}
void UWeaponBase::OnUnequip()
{
	StopFire();
}

void UWeaponBase::StartFire()
{
	if (bIsReloading || bIsFiring) return;
	if (CurrentAmmo <= 0)
	{
		Reload();
		return;
	}
 
	bIsFiring = true;
	FireOnce();
 
	if (FireMode == EWeaponFireMode::FullAuto && OwnerCharacter)
	{
		const float Interval = 1.f / FMath::Max(FireRate, 0.1f);
		OwnerCharacter->GetWorldTimerManager().SetTimer(
			FireTimerHandle, this, &UWeaponBase::FireOnce, Interval, true);
	}
}
void UWeaponBase::StopFire()
{
	bIsFiring = false;
	if (OwnerCharacter)
	{
		OwnerCharacter->GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

void UWeaponBase::Reload()
{
	if (bIsReloading) return;
	if (CurrentAmmo == MagSize) return;
	if (ReserveAmmo <= 0) return;
 
	bIsReloading = true;
	StopFire();
 
	if (ReloadMontage && OwnerCharacter)
	{
		OwnerCharacter->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
	if (ReloadSound && OwnerCharacter)
	{
		UGameplayStatics::PlaySoundAtLocation(OwnerCharacter, ReloadSound,
			OwnerCharacter->GetActorLocation());
	}
	if (OwnerCharacter)
	{
		OwnerCharacter->GetWorldTimerManager().SetTimer(
			ReloadTimerHandle, this, &UWeaponBase::FinishReload, ReloadTime, false);
	}
}

void UWeaponBase::StartAim() { bIsAiming = true; }
void UWeaponBase::StopAim() { bIsAiming = false; }

void UWeaponBase::FireOnce()
{
	if (!OwnerCharacter) return;
	if (CurrentAmmo <= 0)
	{
		StopFire();
		Reload();
		return;
	}
 
	--CurrentAmmo;
 
	if (FireMontage)
	{
		OwnerCharacter->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(FireMontage);
	}
 
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(OwnerCharacter->GetWorld(), MuzzleFlash, GetMuzzleLocation());
	}
 
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(OwnerCharacter, FireSound, GetMuzzleLocation());
	}
 
	switch (WeaponType)
	{
		case EWeaponType::Hitscan: FireHitscan(); break;
		case EWeaponType::Projectile: FireProjectile(); break;
	}
}

void UWeaponBase::FireHitscan()
{
	if (!OwnerCharacter) return;
 
	UWorld* World = OwnerCharacter->GetWorld();
	if (!World) return;
 
	const FVector Start  = GetMuzzleLocation();
	const FVector Direction = GetFireDirection();
	const FVector End = Start + Direction * Range;
 
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	Params.bReturnPhysicalMaterial = true;
 
	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
 
#if WITH_EDITOR
	DrawDebugLine(World, Start, bHit ? HitResult.ImpactPoint : End, FColor::Red, false, 0.5f, 0, 1.f);
#endif
 
	if (bHit && HitResult.GetActor())
	{
		ApplyHitDamage(HitResult.GetActor(), HitResult.ImpactPoint, HitResult.ImpactNormal, HitResult.PhysMaterial.Get());
	}
}

void UWeaponBase::FireProjectile()
{
	if (!OwnerCharacter || !ProjectileClass) return;
 
	UWorld* World = OwnerCharacter->GetWorld();
	if (!World) return;
 
	ABulletPool* Pool = ABulletPool::GetPool(World);
 
	const FVector SpawnLoc = GetMuzzleLocation();
	const FVector Dir = GetFireDirection();
	const FRotator Rot = Dir.Rotation();
 
	if (Pool)
	{
		Pool->SpawnBullet(ProjectileClass, SpawnLoc, Rot, Damage, ProjectileSpeed, OwnerCharacter);
	}
	else
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner= OwnerCharacter;
		SpawnParams.Instigator= OwnerCharacter;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
 
		World->SpawnActor<AActor>(ProjectileClass, SpawnLoc, Rot, SpawnParams);
	}
}

void UWeaponBase::ApplyHitDamage(AActor* HitActor, const FVector& HitLocation, const FVector& HitNormal, UPhysicalMaterial* PhysMat)
{
	if (!HitActor || !OwnerCharacter) return;
 
	AEnemyBase* Enemy = Cast<AEnemyBase>(HitActor);
	if (Enemy)
	{
		Enemy->TakeDamageAmount(Damage);
		return;
	}
	UGameplayStatics::ApplyPointDamage(HitActor, Damage, HitNormal, FHitResult(), OwnerCharacter->GetController(), OwnerCharacter, nullptr);
}

void UWeaponBase::FinishReload()
{
	const int32 Needed = MagSize - CurrentAmmo;
	const int32 Take = FMath::Min(Needed, ReserveAmmo);
	CurrentAmmo += Take;
	ReserveAmmo -= Take;
	bIsReloading = false;
}

 
FVector UWeaponBase::GetMuzzleLocation() const
{
	if (!OwnerCharacter) return FVector::ZeroVector;
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetFirstPersonMesh();
	if (!Mesh) return OwnerCharacter->GetActorLocation();
 
	if (Mesh->DoesSocketExist(FName("Muzzle"))) return Mesh->GetSocketLocation(FName("Muzzle"));
	
	return OwnerCharacter->GetFirstPersonCamera()->GetComponentLocation();
}

FVector UWeaponBase::GetFireDirection() const
{
	if (!OwnerCharacter) return FVector::ForwardVector;
 
	const FVector BaseDir = OwnerCharacter->GetFirstPersonCamera()->GetForwardVector();
 
	const float CurrentSpread = bIsAiming
		? SpreadAngle * AimSpreadModifier
		: SpreadAngle;
 
	if (CurrentSpread <= 0.f) return BaseDir;
 
	const float HalfRad = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	return FMath::VRandCone(BaseDir, HalfRad);
}










