// ANNN PRO


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"



void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//UKismetSystemLibrary::PrintString(this, FString::Printf("ActivateAbility %ls",*GetName()), true, true, FLinearColor::Yellow, 5.0f);
	
	
	
}
