// ANNN PRO


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	const AAuraPlayerState* UAuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);


	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
	OnPlayerXPChanged(UAuraPlayerState->GetPlayerXP());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* UAuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	UAuraPlayerState->OnLevelChanged.AddUObject(this, &UOverlayWidgetController::OnPlayerLevelChanged);
	UAuraPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnPlayerXPChanged);
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData)
		{
			OnHealthChanged.Broadcast(OnAttributeChangeData.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData){
			OnMaxHealthChanged.Broadcast(OnAttributeChangeData.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData){
			OnManaChanged.Broadcast(OnAttributeChangeData.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData){
			OnMaxManaChanged.Broadcast(OnAttributeChangeData.NewValue);
			});

	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		//监听能力的激活 在Overlay上表示
		if (AuraAbilitySystemComponent->bStartAbilityGiven)
		{
			//如果已经激活直接调用， 否则监听调用
			OnInitializeStartUpAbilities(AuraAbilitySystemComponent);
		}
		else
		{
			AuraAbilitySystemComponent->OnAbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartUpAbilities);
		}
		
		AuraAbilitySystemComponent->OnEffectAppliedTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& Tags)
		{
			for (FGameplayTag Tag : Tags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (!Tag.MatchesTag(MessageTag)) return;
				
				const FString ContextString = FString::Printf(TEXT("GE TAG: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ContextString);
				const FUIWidgetRow* Row = GetDataFromDataTable<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				OnMessageWidgetRow.Broadcast(*Row);
			}
		});

	}
	
	
}

void UOverlayWidgetController::OnInitializeStartUpAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	check(AuraAbilitySystemComponent);
	if (!AuraAbilitySystemComponent->bStartAbilityGiven) return;

	FForEachAbility Func;
	Func.BindLambda([this](const FGameplayAbilitySpec& Spec)
	{
		FAuraAbilityInfo InfoData = AbilityInfoData->FindAbilityInfoForTag(UAuraAbilitySystemComponent::GetAbilityTagFromAbilitySpec(Spec));
		InfoData.InputTag = UAuraAbilitySystemComponent::GetInputTagFromAbilitySpec(Spec);
		OnAbilityInfoSet.Broadcast(InfoData);
		
	});
	//遍历当前的所有能力并设置InputTag和通知overlay
	AuraAbilitySystemComponent->ForEachAbility(Func);
}


void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewLevel)
{
	const AAuraPlayerState* UAuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnLevelChanged.Broadcast(UAuraPlayerState->GetPlayerLevel());

}

void UOverlayWidgetController::OnPlayerXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* UAuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = UAuraPlayerState->LevelUpInfo;;
	checkf(LevelUpInfo, TEXT("LevelUpInfo is null"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	
	if (Level<= MaxLevel && Level > 0)
	{
		const int32 LevelUpReq = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreLevelUpReq = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 XPDelta = LevelUpReq - PreLevelUpReq;
		const int32 XPThisLevel = NewXP - PreLevelUpReq;
		const int32 XPNextLevel = LevelUpReq - NewXP;
		const int XPPercent = XPThisLevel / XPDelta;

		OnXPChanged.Broadcast(XPPercent);
	}
	
	
}
