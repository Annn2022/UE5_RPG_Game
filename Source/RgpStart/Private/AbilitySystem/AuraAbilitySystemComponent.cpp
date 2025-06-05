// ANNN PRO


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "RgpStart/AuraLogChannels.h"

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
	bStartAbilityGiven = true;
	OnAbilitiesGivenDelegate.Broadcast(this);
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

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Func)
{
	//遍历时锁定能力系统
	FScopedAbilityListLock ActiveAbilitiesLock(*this);
	for (auto& Spec : GetActivatableAbilities())
	{
		if (!Func.ExecuteIfBound(Spec))
		{
			UE_LOG(LogAura, Warning, TEXT("Failed Found Ability: %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	if (Spec.Ability)
	{
		for (FGameplayTag Tag: Spec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
				
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	
	for (FGameplayTag Tag: Spec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartAbilityGiven)
	{
		bStartAbilityGiven = true;
		OnAbilitiesGivenDelegate.Broadcast(this);

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

