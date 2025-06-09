// ANNN PRO


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FAuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraGASLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	//主属性
	AllAttributesForTag.Add(GameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Primary_Resilience, GetResilienceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Primary_Vigor, GetVigorAttribute);

	//次要属性
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_Armor, GetArmorAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_CritcalHitChance, GetCritcalHitChanceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_CritcalHitDamage, GetCritcalHitDamageAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_CritcalHitResistance, GetCritcalHitResistanceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	
	//属性抗性
	AllAttributesForTag.Add(GameplayTags.Attribute_Resistance_Fire, GetFireResistanceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Resistance_Arcane, GetArcaneResistanceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Resistance_Lightning, GetLightningResistanceAttribute);
	AllAttributesForTag.Add(GameplayTags.Attribute_Resistance_Physical, GetPhysicalResistanceAttribute);

}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//确保网络会复制的值
	//主属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	//次要属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritcalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritcalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritcalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	//属性抗性
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	//基础属性
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

FGameplayAttribute UAuraAttributeSet::FindAttributeByTag(FGameplayTag Tag) const
{
	for (const auto& Pair : AllAttributesForTag)
	{
		if (Pair.Key == Tag)
		{
			return Pair.Value();
		}
	}
	return FGameplayAttribute();
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetMaxHealthAttribute() && bIsTopOffHealth)
	{
		bIsTopOffHealth = false;
		SetHealth(GetMaxHealth());
	}
	else if (Attribute == GetMaxManaAttribute() && bIsTopOffMana)
	{
		bIsTopOffMana = false;

		SetMana(GetMaxMana());
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& oldHeath) const
{
	//当网络复制本地的值后，会进行广播，确保所有客户端会接受
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, oldHeath);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHeath) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, oldMaxHeath);

}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& oldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, oldMana);

}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& oldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, oldMaxMana);

}

inline void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& oldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, oldStrength);

}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& oldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, oldIntelligence);

}

inline void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& oldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, oldResilience);

}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& oldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, oldVigor);

}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& oldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, oldFireResistance);

}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& oldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, oldLightningResistance);

}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& oldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, oldArcaneResistance);

}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& oldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, oldPhysicalResistance);

}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& oldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, oldArmor);

}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& oldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, oldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& oldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, oldBlockChance);

}

void UAuraAttributeSet::OnRep_CritcalHitChance(const FGameplayAttributeData& oldCritcalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritcalHitChance, oldCritcalHitChance);

}

void UAuraAttributeSet::OnRep_CritcalHitDamage(const FGameplayAttributeData& oldCritcalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritcalHitDamage, oldCritcalHitDamage);

}

void UAuraAttributeSet::OnRep_CritcalHitResistance(const FGameplayAttributeData& oldCritcalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritcalHitResistance, oldCritcalHitResistance);

}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& oldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, oldHealthRegeneration);

}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& oldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, oldManaRegeneration);

}


/*
 * 属性被修改前触发，一般用于数值校验和限制
 */
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, NewValue);
	}
}


void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceAbilitySystemComponent = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceAbilitySystemComponent)
		&& Props.SourceAbilitySystemComponent->AbilityActorInfo.IsValid()
		&& Props.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceActor = Props.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());

		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetActor);
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetActor);
	}
	
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockHit, bool bCriticalHit) const
{
	if (AAuraPlayerController* SourceController =
				Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
	{
		SourceController->ShowDamageText(Damage,Cast<ACharacter>(Props.TargetActor), bBlockHit,bCriticalHit);
	}
	else if (AAuraPlayerController* TargetController =
				Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
	{
		TargetController->ShowDamageText(Damage,Cast<ACharacter>(Props.TargetActor), bBlockHit,bCriticalHit);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties EffectProperties;
	SetEffectProperties(Data,EffectProperties);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp<float>(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Health Changed %f on %s"), GetHealth(), *EffectProperties.TargetActor->GetName());
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp<float>(GetMana(), 0.f, GetMaxMana()));
	}

	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (Damage > 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage Taken %f on %s"), Damage, *EffectProperties.TargetActor->GetName());
			const float NewHealth = GetHealth() - Damage;
			SetHealth(FMath::Clamp<float>(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				EffectProperties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetActor))
				{
					CombatInterface->Die();
					SendXPEvent(EffectProperties);
				}
			}

			
			ShowFloatingText(EffectProperties, Damage,
				UAuraGASLibrary::IsBlockedHit(EffectProperties.EffectContextHandle),
				UAuraGASLibrary::IsCriticalHit(EffectProperties.EffectContextHandle));
		}
	}

	else if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float XP = GetIncomingXP();
		SetIncomingXP(0.f);
		UE_LOG(LogTemp, Warning, TEXT("%s 获得经验： %f "), *EffectProperties.TargetActor->GetName(),XP);
		 if (EffectProperties.SourceCharacter->Implements<UPlayerInterface>()
		 	&& EffectProperties.SourceCharacter->Implements<UCombatInterface>())
		 {
		 	const int32 CurLevel = ICombatInterface::Execute_GetActorLevel(EffectProperties.SourceCharacter);
		 	const int32 CurXP = IPlayerInterface::Execute_GetXP(EffectProperties.SourceCharacter);
		 	
		 	const int32 NewLevel = IPlayerInterface::Execute_FindNewLevelForXP(EffectProperties.SourceCharacter, CurXP + XP);
		 	const int32 AddLevel = NewLevel - CurLevel;
		 	if (AddLevel > 0)
		 	{
		 		//TODO 未考虑连升等级的情况
		 		const int32 AttrPointsReward = IPlayerInterface::Execute_GetAttributePointsReward(EffectProperties.SourceCharacter, CurLevel);
		 		const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(EffectProperties.SourceCharacter, CurLevel);
		 		IPlayerInterface::Execute_AddToLevel(EffectProperties.SourceCharacter, AddLevel);
		 		IPlayerInterface::Execute_AddToAttributePoint(EffectProperties.SourceCharacter, AttrPointsReward);
		 		IPlayerInterface::Execute_AddToSpellPoint(EffectProperties.SourceCharacter, SpellPointsReward);

		 		bIsTopOffHealth = true;
		 		bIsTopOffMana = true;
		 		
		 		IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);
		 	
		 	}
		 	IPlayerInterface::Execute_AddToXP(EffectProperties.SourceCharacter,XP);
		 }
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props) const
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetActorLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UAuraGASLibrary::GetRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attribute_Meta_IncomingXP, Payload);
	}
}



