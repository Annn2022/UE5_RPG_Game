// ANNN PRO


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAS = GetAuraAS();
	const AAuraPlayerState* AuraPS = GetAuraPS();


	OnHealthChanged.Broadcast(AuraAS->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAS->GetMana());
	OnMaxManaChanged.Broadcast(AuraAS->GetMaxMana());
	
	OnPlayerXPChanged(AuraPS->GetPlayerXP());
	OnPlayerLevelChanged(AuraPS->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* AuraPS = GetAuraPS();
	UAuraAttributeSet* AuraAS = GetAuraAS();

	AuraPS->OnLevelChanged.AddUObject(this, &UOverlayWidgetController::OnPlayerLevelChanged);
	AuraPS->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnPlayerXPChanged);

	// UAuraPlayerState->OnLevelChanged.AddLambda(
	// 	[this](int32 NewLevel)
	// 	{
	// 		OnPlayerStateChanged.Broadcast(NewLevel);
	// 	});
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAS->GetHealthAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData)
		{
			OnHealthChanged.Broadcast(OnAttributeChangeData.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAS->GetMaxHealthAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData){
			OnMaxHealthChanged.Broadcast(OnAttributeChangeData.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAS->GetManaAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData){
			OnManaChanged.Broadcast(OnAttributeChangeData.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAS->GetMaxManaAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& OnAttributeChangeData){
			OnMaxManaChanged.Broadcast(OnAttributeChangeData.NewValue);
			});

	UAuraAbilitySystemComponent* AuraASC = GetAuraASC();
	//监听能力的激活 在Overlay上表示
	if (AuraASC->bStartAbilityGiven)
	{
		//如果已经激活直接调用， 否则监听调用
		BroadcastAbilityInfo();
	}
	else
	{
		AuraASC->OnAbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
	}
	
	AuraASC->OnEffectAppliedTagsDelegate.AddLambda(
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



void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewLevel) const
{
	OnLevelChanged.Broadcast(NewLevel);
}

void UOverlayWidgetController::OnPlayerXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;;
	checkf(LevelUpInfo, TEXT("LevelUpInfo is null"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	
	if (Level<= MaxLevel && Level > 0)
	{
		const int32 LevelUpReq = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreLevelUpReq = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const float XPDelta = LevelUpReq - PreLevelUpReq;
		const float XPThisLevel = NewXP - PreLevelUpReq;
		const float XPNextLevel = LevelUpReq - NewXP;
		const float XPPercent = XPThisLevel / XPDelta;

		OnXPChanged.Broadcast(XPPercent);
	}
	
	
}
