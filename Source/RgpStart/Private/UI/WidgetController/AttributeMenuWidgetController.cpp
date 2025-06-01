// ANNN PRO


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "FAuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfos.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(MyAttributeInfo)

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
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(MyAttributeInfo)
	
	for (auto& Pair : AS->AllAttributesForTag)
	{
		FAuraAttributeInfo Info = MyAttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.BaseValue = Pair.Value().GetNumericValue(AS);
        AttributeInfoDelegate.Broadcast(Info);
    }
	
}
