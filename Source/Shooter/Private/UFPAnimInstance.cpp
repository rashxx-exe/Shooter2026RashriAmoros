#include "UFPAnimInstance.h"
#include "FPCharacter.h"
#include "UWeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UFPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter=Cast<AFPCharacter>(TryGetPawnOwner());
}

void UFPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!OwnerCharacter)
	{
		OwnerCharacter=Cast<AFPCharacter>(TryGetPawnOwner());
		if(!OwnerCharacter)return;
	}

	const FVector Velocity=OwnerCharacter->GetVelocity();

	Speed=Velocity.Size2D();
	bIsInAir=OwnerCharacter->GetCharacterMovement()->IsFalling();
	bIsSprinting=OwnerCharacter->GetIsSprinting();

	Direction=UKismetMathLibrary::NormalizedDeltaRotator(
		Velocity.Rotation(),
		OwnerCharacter->GetActorRotation()
	).Yaw;

	bIsAiming=OwnerCharacter->GetIsAiming();
	bIsDead=OwnerCharacter->GetIsDead();

	const UWeaponBase* Weapon=OwnerCharacter->GetCurrentWeapon();

	if(Weapon)
	{
		bIsFiring=Weapon->GetIsFiring();
		bIsReloading=Weapon->GetIsReloading();
	}
	else
	{
		bIsFiring=false;
		bIsReloading=false;
	}
}