// ANNN PRO


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraGASLibrary.h"

float UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	if (!UAuraGASLibrary::IsNotFriend(GetAvatarActorFromActorInfo(), TargetActor))
	{
		return -1.f;
	}
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	float TotalDamage = 0.f;
	for (TTuple<FGameplayTag, FScalableFloat> Pairs : DamageTypes)
	{
		const float ScaledDamage = Pairs.Value.GetValueAtLevel(GetAbilityLevel());

		//将Tag&Value 赋值给SpecHandle
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pairs.Key, ScaledDamage);
		TotalDamage += ScaledDamage;
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	return TotalDamage;
}
