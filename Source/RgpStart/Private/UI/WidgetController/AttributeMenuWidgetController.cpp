// ANNN PRO


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfos.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = GetAuraAS();

	for (auto& Pair : AS->AllAttributesForTag)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
		.AddLambda(
			[this,Pair,AS](const  FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = MyAttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.BaseValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);
			});
	}

	AAuraPlayerState* UAuraPlayerState = GetAuraPS();
	
	UAuraPlayerState->OnAttributePointChanged.AddLambda([this](int32 NewPoints)
	{
		OnAttributePointChangedDelegate.Broadcast(NewPoints);
	});

	UAuraPlayerState->OnSpellPointChanged.AddLambda([this](int32 NewPoints)
	{
		OnSpellPointChangedDelegate.Broadcast(NewPoints);
	});
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetAuraAS()->AllAttributesForTag)
	{
		FAuraAttributeInfo Info = MyAttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.BaseValue = Pair.Value().GetNumericValue(GetAuraAS());
        AttributeInfoDelegate.Broadcast(Info);
    }
	

	OnAttributePointChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoint());
	OnSpellPointChangedDelegate.Broadcast(GetAuraPS()->GetPlayerSpellPoint());

	
}

bool UAttributeMenuWidgetController::AddAttributeValue(const FGameplayTag& AttributeTag, const int32 AddNum)
{
	
	if (GetAuraPS()->GetAttributePoint() < AddNum)
	{
		//属性点不足
		return false;
	}
	
	return GetAuraASC()->UpgradeAttribute(AttributeTag);
}
