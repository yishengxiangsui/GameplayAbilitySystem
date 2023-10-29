// This Project Made By Yuan Xiaodong.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
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

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	//获取controller的Yaw旋转值
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	
	//获取controller的向前向量和向右向量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	//移动输入，Controller向前对应键盘的WS键，因此是InputActionValue的Y轴。
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	/**
	 *路径追踪时有以下几种情况：
	 *1. LastActor is null && ThisActor is null
	 *		-什么都不做
	 *2. LastActor is null && ThisActor is Valid
	 *		-高亮ThisActor
	 *3. LastActor is Valid && ThisActor is null
	 *		-取消LastActor的高亮
	 *4. LastActor is Valid && ThisActor is Valid, But LastActor != ThisActor
	 *		-取消LastActor的高亮，高亮ThisActor
	 *5. LastActor、ThisActor are Valid， And are the same actor
	 *		-什么都不做
	 */
	 if(LastActor == nullptr)
	 {
		 if(ThisActor != nullptr)
		 {
			 // 2.  -LastActor is null && ThisActor is Valid
		 	ThisActor->HighlightActor();
		 }
	 	else
	 	{
	 		// 1.  -LastActor is null && ThisActor is null
	 		//do nothing
	 	}
	 }
	else //LastActor is Valid
	{
		if(ThisActor == nullptr)
		{
			// 3.  -LastActor is Valid && ThisActor is null
			LastActor->UnHighlightActor();
		}
		else //ThisActor is Valid
		{
			if(LastActor != ThisActor)
			{
				//4.  -LastActor is Valid && ThisActor is Valid, But LastActor != ThisActor
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else //5.  -LastActor、ThisActor are Valid， And are the same actor
			{
				//do nothing
			}
		}
	}
}
