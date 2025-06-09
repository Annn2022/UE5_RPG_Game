// ANNN PRO


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnSpellPointChangedDelegate.Broadcast(GetAuraPS()->GetPlayerSpellPoint());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
	{
		if (AbilityInfoData)
		{
			FAuraAbilityInfo Info =AbilityInfoData->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			Info.LevelRequirement = AbilityLevel;
			OnAbilityInfoSet.Broadcast(Info);
		}
	});

	GetAuraPS()->OnSpellPointChanged.AddLambda([this](const int32 SpellPoint)
	{
		OnSpellPointChangedDelegate.Broadcast(SpellPoint);
	});
}
