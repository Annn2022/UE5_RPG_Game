// ANNN PRO


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FAuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "HAL/Platform.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

	AutoRun();
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraMappingContext);

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
if (PlayerSubsystem)
{
	PlayerSubsystem->AddMappingContext(AuraMappingContext,0);
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
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	check(AuraInputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(AuraInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::ShowDamageText_Implementation(float Damage, ACharacter* DamagedActor, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(DamagedActor) && DamageTextClass && IsLocalController())
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(DamagedActor, DamageTextClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(DamagedActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(Damage, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	
	const FVector2D InputAxisVector2 = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0,Rotation.Yaw,0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector2.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector2.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	if (!HitResult.bBlockingHit) return;
	LastHitActor = ThisHitActor;
	ThisHitActor = HitResult.GetActor();

	//enemyInterface
	IEnemyInterface* lastEnemy= Cast<IEnemyInterface>(LastHitActor);
	IEnemyInterface* thisEnemy= Cast<IEnemyInterface>(ThisHitActor);

	if (lastEnemy != thisEnemy)
	{
		if (lastEnemy) lastEnemy->UnHighLightActor();
		if (thisEnemy) thisEnemy->HighLightActor();
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisHitActor != nullptr &&Cast<IEnemyInterface>(ThisHitActor) != nullptr;
		bAutoRunning = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())
			GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetAuraAbilitySystemComponent())
		GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting & !bShiftPressed)
	{
		//移动
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously( this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLocation : Path->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation,ESplineCoordinateSpace::World);
					DrawDebugSphere( GetWorld(), PointLocation, 8.0f, 8, FColor::Green, false, 5.f);
				}
				if (Path->PathPoints.Num() > 1)
				{
					CachedDestination = Path->PathPoints[Path->PathPoints.Num()-1];
				}
				bAutoRunning = true;
			}
			
		}
		FollowTime =0;
		bTargeting = false;
		
	}
	
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		return;
	}
	if (bTargeting || bShiftPressed)
	{
		if (GetAuraAbilitySystemComponent())
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		
	}
	else
	{
		//点击移动逻辑
		FollowTime+=GetWorld()->DeltaTimeSeconds;

		//在playerTick中更新了HitResult 此处更新目的地
		if (HitResult.bBlockingHit)
		{
			CachedDestination = HitResult.ImpactPoint;
		}

		if(APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection,1.0f);
		}
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationAtSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		//获取曲线在该点的切线（平滑转向
		const FVector Direction =  Spline->FindDirectionClosestToWorldLocation(LocationAtSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction,1.0f);

		const float DistanceToDestination = (LocationAtSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

