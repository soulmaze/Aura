// Copyright Soulmaze Compony


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor == nullptr) //LastActor invalid
	{
		if (ThisActor != nullptr)
		{
			//&& ThisActor is valid -> Highlight ThisActor
			ThisActor->HighlightActor();
		}
		else
		{
			//&& ThisActor is invalid -> do nothing
		}
	}
	else //LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// && ThisActor is invalid too -> unhighlight LastActor
			LastActor->UnhighlightActor();
		}
		else //&& ThisActors is valid too
		{
			if (LastActor != ThisActor) 
			{
				//but they are not the same
				LastActor->UnhighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//But they are the same -> do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(SubSystem)
	{
		SubSystem->AddMappingContext(AuraMappingContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;

	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotation(0, Rotator.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection * InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection * InputAxisVector.X);
	}
}

