// ANNN PRO


#include "Character/AuraEnemy.h"

#include "FAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraGASLibrary.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RgpStart/RgpStart.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	/*
	 * 当设置为true时，表示角色移动时会自动旋转以匹配控制器的期望朝向（即控制器输入的方向）。对于AI控制的敌人，这意味着它会自动面向其移动方向。
	 */
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AIController 只在服务器运行
	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(GetController());
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HighLightActor()
{
	bHighLighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighLightActor()
{
	bHighLighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	
}

int32 AAuraEnemy::GetActorLevel_Implementation() const
{return Level;}

void AAuraEnemy::Die()
{
	SetLifeSpan(DeathTimeSpan);
	if (AuraAIController) 
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);

	Super::Die();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* NewTarget)
{
	CombatTarget = NewTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::HitReactTahChanged(const FGameplayTag EventTag, int32 NewCount)
{
	bIsHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		//AuraAIController 只会在服务器运行
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bIsHitReacting);
	}

}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	UAuraGASLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	
	if (UAuraUserWidget* UW = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		UW->SetWidgetController(this);
	}
	
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AAuraEnemy::HitReactTahChanged);
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
	
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(HasAuthority())
	{
		InitDefaultAttributes();
	}
	
}

void AAuraEnemy::InitDefaultAttributes() const
{
	UAuraGASLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}
