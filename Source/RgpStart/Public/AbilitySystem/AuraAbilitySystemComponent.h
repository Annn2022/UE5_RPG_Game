

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)
/**
 * 
 */
UCLASS()
class RGPSTART_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	FEffectAssetTags OnEffectAppliedTagsDelegate;
	FAbilitiesGiven OnAbilitiesGivenDelegate;
	bool bStartAbilityGiven =false;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	void AbilityInputTagHeld(const FGameplayTag& Tag);
	void AbilityInputTagReleased(const FGameplayTag& Tag);
	void ForEachAbility(const FForEachAbility& Func);
	static FGameplayTag GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetInputTagFromAbilitySpec(const FGameplayAbilitySpec& Spec);

protected:
	UFUNCTION(Client, Reliable) //确保客户端收到
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);

	virtual void OnRep_ActivateAbilities() override;
};

