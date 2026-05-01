#include "UGameInputs.h"
#include  "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Slate/SGameLayerManager.h"

UUGameInputs::UUGameInputs()
{
	bWantsInitializeComponent = true;
}

void UGameInputs::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		UEnhancedInputSubsystemInterface* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputSubsystemInterface>(PlayerController->GetLocalPlayer());
		if (InputSubsystem)
		{
			InputSubsystem->AddMappingContext(UInputMappingContext, 0);
		}
	}

	if (LefClick)
	{
		BindAction(LefClick, ETriggerEvent::Triggered, this, &UGameInputs::OnLeftClick)
	}

	if (RightClick)
	{
		BindAction(RightClick, ETriggerEvent::Triggered, this, &UGameInputs::OnRightClick)
	}

	if (MoveAction)
	{
		BindAction(MoveAction, ETriggerEvent::Triggered, this, &UGameInputs::MoveInput)
	}
	
}
