#include "AShooterHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AShooterHUD::AShooterHUD()
{
	bIsPaused=false;
}

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowMainHUD();
}

void AShooterHUD::ShowMainHUD()
{
	if(!MainHUDClass)return;

	if(!MainHUDWidget)
	{
		MainHUDWidget=CreateWidget<UUserWidget>(
			GetOwningPlayerController(),
			MainHUDClass
		);
	}

	if(MainHUDWidget&&!MainHUDWidget->IsInViewport())
	{
		MainHUDWidget->AddToViewport();
	}
}

void AShooterHUD::TogglePauseMenu()
{
	if(!PauseMenuClass)return;

	bIsPaused=!bIsPaused;

	UGameplayStatics::SetGamePaused(GetWorld(),bIsPaused);

	APlayerController* PC=GetOwningPlayerController();

	if(bIsPaused)
	{
		if(!PauseMenuWidget)
		{
			PauseMenuWidget=CreateWidget<UUserWidget>(
				PC,
				PauseMenuClass
			);
		}

		if(PauseMenuWidget&&!PauseMenuWidget->IsInViewport())
		{
			PauseMenuWidget->AddToViewport(10);
		}

		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		if(PauseMenuWidget&&PauseMenuWidget->IsInViewport())
		{
			PauseMenuWidget->RemoveFromParent();
		}

		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void AShooterHUD::ShowDeathScreen()
{
	if(!DeathScreenClass)return;

	if(!DeathScreenWidget)
	{
		DeathScreenWidget=CreateWidget<UUserWidget>(
			GetOwningPlayerController(),
			DeathScreenClass
		);
	}

	if(DeathScreenWidget&&!DeathScreenWidget->IsInViewport())
	{
		DeathScreenWidget->AddToViewport(20);
	}

	APlayerController* PC=GetOwningPlayerController();

	if(PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}