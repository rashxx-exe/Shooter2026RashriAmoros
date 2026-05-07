#include "AShooterGameMode.h"
#include "ABulletPool.h"
#include "FPCharacter.h"
#include "AShooterHUD.h"
#include "Engine/World.h"

AShooterGameMode::AShooterGameMode()
{
	DefaultPawnClass=AFPCharacter::StaticClass();
	HUDClass=AShooterHUD::StaticClass();
	Score=0;
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(BulletPoolClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		BulletPoolInstance=GetWorld()->SpawnActor<ABulletPool>(
			BulletPoolClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);
	}
}

void AShooterGameMode::AddScore(int32 Amount)
{
	Score+=Amount;
}

void AShooterGameMode::OnPlayerDied()
{
	AShooterHUD* HUD=Cast<AShooterHUD>(
		GetWorld()->GetFirstPlayerController()->GetHUD()
	);

	if(HUD)
	{
		HUD->ShowDeathScreen();
	}
}