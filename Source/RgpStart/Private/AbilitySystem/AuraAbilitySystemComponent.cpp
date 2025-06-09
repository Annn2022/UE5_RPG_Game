// ANNN PRO


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FAuraGameplayTags.h"
#include "AbilitySystem/AuraGASLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"
#include "RgpStart/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//绑定委托 Effect Apply时调用
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraGASLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag,FAuraGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();

}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();

}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	return FGameplayTag();
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
	OnAbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const auto InAbilityClass : Abilities)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(InAbilityClass,1);
		GiveAbilityAndActivateOnce(Spec);
		
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

bool UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePointCount(GetAvatarActor())> 0)
		{
			ServerUpgradeAttribute(AttributeTag);
			return true;
		}
	}
	return false;

}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute(const FGameplayTag& AttributeTag)
{
	FGameplayEventData EventData;
	EventData.EventTag = AttributeTag;
	EventData.EventMagnitude = 1;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag,EventData);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoint(GetAvatarActor(),-1);
		
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartAbilityGiven)
	{
		bStartAbilityGiven = true;
		OnAbilitiesGivenDelegate.Broadcast();

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

