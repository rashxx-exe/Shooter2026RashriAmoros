#pragma once

#include "EnhancedInputComponent.h"
#include "FPCharacter.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class UGameInputs : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UGameInputs();

protected:
	virtual void BeginPlay();
	
	UPROPERTY(EditAnywhere, Category = "GameInputs")
	UInputAction* LefClick;

	UPROPERTY(EditAnywhere, Category = "GameInputs")
	UInputAction* RightClick;

	UPROPERTY(EditAnywhere, Category = "GameInputs")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "GameInputs")
	UInputAction* MouseLookAction;

	void OnLeftClick(const FInputActionValue& Value)
	{
		
	}

	void OnRightClick(const FInputActionValue& Value)
	{
		
	}

	void MoveInput(const FInputActionValue& Value)
	{
		FVector2D MoveVec = Value.Get<FVector2D>();
		AFPCharacter::DoMove(MoveVec.X, MoveVec.Y);
	}
};