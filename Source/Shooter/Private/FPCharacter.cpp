#include "FPCharacter.h"
#include "FPCharacter.h"
 
#include "UWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
 
AFPCharacter::AFPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 96.f);

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	FPMesh->SetupAttachment(GetMesh());
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FPMesh->SetCollisionProfileName(FName("NoCollision"));
	FPMesh->CastShadow = false;

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(FPMesh, FName("head"));
	FPCamera->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.f), FRotator(0.f, 90.f, -90.f));
	FPCamera->bUsePawnControlRotation = true;
	FPCamera->bEnableFirstPersonFieldOfView = true;
	FPCamera->FirstPersonFieldOfView = 70.f;
	FPCamera->FirstPersonScale = 0.6f;

	WalkSpeed = 600.f;
	SprintSpeed = 900.f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 550.f;
	GetCharacterMovement()->AirControl = 0.35f;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	bIsSprinting = false;
	bIsAiming = false;
	bIsDead = false;

	CurrentWeaponIndex = 0;
	
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	InitWeapons();
}

void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EIC) return;

	if (MoveAction) EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPCharacter::OnMove);
	if (LookAction) EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPCharacter::OnLook);
	if (JumpAction)
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPCharacter::OnJumpStarted);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPCharacter::OnJumpCompleted);
	}
	if (FireAction)
	{
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &AFPCharacter::OnFireStarted);
		EIC->BindAction(FireAction, ETriggerEvent::Completed, this, &AFPCharacter::OnFireCompleted);
	}
	if (AimAction)
	{
		EIC->BindAction(AimAction, ETriggerEvent::Started, this, &AFPCharacter::OnAimStarted);
		EIC->BindAction(AimAction, ETriggerEvent::Completed, this, &AFPCharacter::OnAimCompleted);
	}
	if (ReloadAction) EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &AFPCharacter::OnReload);
	if (SwitchWeaponAction) EIC->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &AFPCharacter::OnSwitchWeapon);
	if (SprintAction)
	{
		EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AFPCharacter::OnSprintStarted);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFPCharacter::OnSprintCompleted);
	}
}

void AFPCharacter::DoMove(float Forward, float Right)
{
	if (!GetController()) return;
	AddMovementInput(GetActorForwardVector(), Forward);
	AddMovementInput(GetActorRightVector(),   Right);
}
void AFPCharacter::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void AFPCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	DoMove(MoveVec.Y, MoveVec.X);
}
void AFPCharacter::OnLook(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();
	DoLook(LookVec.X, LookVec.Y);
}
void AFPCharacter::OnJumpStarted(const FInputActionValue& Value)  { Jump(); }
void AFPCharacter::OnJumpCompleted(const FInputActionValue& Value) { StopJumping(); }
void AFPCharacter::OnFireStarted(const FInputActionValue& Value)
{
	if (CurrentWeapon) CurrentWeapon->StartFire();
}
void AFPCharacter::OnFireCompleted(const FInputActionValue& Value)
{
	if (CurrentWeapon) CurrentWeapon->StopFire();
}
void AFPCharacter::OnAimStarted(const FInputActionValue& Value)
{
	bIsAiming = true;
	if (CurrentWeapon) CurrentWeapon->StartAim();
}
void AFPCharacter::OnAimCompleted(const FInputActionValue& Value)
{
	bIsAiming = false;
	if (CurrentWeapon) CurrentWeapon->StopAim();
}
void AFPCharacter::OnReload(const FInputActionValue& Value)
{
	if (CurrentWeapon) CurrentWeapon->Reload();
}
void AFPCharacter::OnSwitchWeapon(const FInputActionValue& Value)
{
	if (Weapons.Num() <= 1) return;
	int32 Next = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(Next);
}
void AFPCharacter::OnSprintStarted(const FInputActionValue& Value)
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void AFPCharacter::OnSprintCompleted(const FInputActionValue& Value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFPCharacter::InitWeapons()
{
	for (TSubclassOf<UWeaponBase> WeaponClass : WeaponsClasses)
	{
		if (!WeaponClass) continue;
		UWeaponBase* NewWeapon = NewObject<UWeaponBase>(this, WeaponClass);
		if (NewWeapon)
		{
			NewWeapon->InitWeapon(this);
			Weapons.Add(NewWeapon);
		}
	}
	if (Weapons.Num() > 0)
	{
		EquipWeapon(0);
	}
}

void AFPCharacter::EquipWeapon(int32 Index)
{
	if (!Weapons.IsValidIndex(Index)) return;
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		CurrentWeapon->OnUnequip();
	}
	CurrentWeaponIndex = Index;
	CurrentWeapon = Weapons[Index];
	CurrentWeapon->OnEquip();
}

void AFPCharacter::TakeDamageAmount(float Damage)
{
	if (bIsDead) return;
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	if (CurrentHealth <= 0.f)
	{
		Die();
	}
}

void AFPCharacter::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	if (CurrentWeapon) CurrentWeapon->StopFire();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


 



