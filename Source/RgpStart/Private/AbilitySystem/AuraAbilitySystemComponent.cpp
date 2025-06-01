// ANNN PRO


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//绑定委托 Effect Apply时调用
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const auto InAbilityClass : Abilities)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(InAbilityClass,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(Spec.Ability))
		{
			Spec.GetDynamicSpecSourceTags().AddTag(AuraGameplayAbility->StartupInputTag);
			GiveAbility(Spec);

		}
	}
}

//测试用
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (auto& Spec : GetActivatableAbilities()) //获取所有已启用（不要和激活混淆）的AbilitySpec
		{
			if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) //需要精确匹配
			{
				AbilitySpecInputPressed(Spec); //通知能力系统该能力的输入已被按下
				if (!Spec.IsActive())
				{
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

//测试用
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (auto& Spec : GetActivatableAbilities())
		{
			if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				AbilitySpecInputReleased(Spec);
				
			}
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer Tags;
	EffectSpec.GetAllAssetTags(Tags);

	//呼出Effect的所有Tags
	OnEffectAppliedTagsDelegate.Broadcast(Tags);
	
}
