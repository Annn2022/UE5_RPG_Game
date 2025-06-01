// ANNN PRO


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
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
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAppliedTagsDelegate.AddLambda(
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
