// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//定义一个有返回值的委托
//DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature)

//别名 返回值为FGameplayAttribute的函数指针
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;

//创建一个有返回值（类型为T）的函数指针模板
template<class T>
using FStaticFuncPtr = typename TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle = {};

	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;

	UPROPERTY()
	AActor* SourceActor  = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

	UPROPERTY()
	AActor* TargetActor = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;
	
};

/**
 * 
 */
UCLASS()
class RGPSTART_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	TMap<FGameplayTag, FStaticFuncPtr<FGameplayAttribute()>> AllAttributesForTag;;
	/*
 * 主要数值
 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength,Category="主要数值")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence,Category="主要数值")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience,Category="主要数值")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor,Category="主要数值")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
/*
 * 基础数值
 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,Category="基础数值")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,Category="基础数值")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana,Category="基础数值")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana,Category="基础数值")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	/*
* 次要数值
*/
	//护甲 抗性加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor,Category="次要数值")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	//护甲穿透 抗性加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration,Category="次要数值")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);
	
	//格挡率 伤害减半 护甲加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance,Category="次要数值")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	//暴击率 护甲穿透加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritcalHitChance,Category="次要数值")
	FGameplayAttributeData CritcalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritcalHitChance);

	//暴击伤害 护甲穿透加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritcalHitDamage,Category="次要数值")
	FGameplayAttributeData CritcalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritcalHitDamage);

	//暴击抵抗 护甲加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritcalHitResistance,Category="次要数值")
	FGameplayAttributeData CritcalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritcalHitResistance);

	//生命恢复 活力加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration,Category="次要数值")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	//魔法恢复 活力加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration,Category="次要数值")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	/*
	 * 各种抗性
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance,Category="次要数值")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance,Category="次要数值")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance,Category="次要数值")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance,Category="次要数值")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	/*
	 * Meta Attributes
	 * 仅在服务器上计算，无需复制
	 */
	UPROPERTY(BlueprintReadOnly,Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& oldHeath) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHeath) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& oldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& oldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& oldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& oldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& oldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& oldVigor) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& oldFireResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& oldLightningResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& oldArcaneResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& oldPhysicalResistance) const;

	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& oldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& oldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& oldBlockChance) const;

	UFUNCTION()
	void OnRep_CritcalHitChance(const FGameplayAttributeData& oldCritcalHitChance) const;

	UFUNCTION()
	void OnRep_CritcalHitDamage(const FGameplayAttributeData& oldCritcalHitDamage) const;

	UFUNCTION()
	void OnRep_CritcalHitResistance(const FGameplayAttributeData& oldCritcalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& oldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& oldManaRegeneration) const;
protected:

	/*
	 * 继承自UAttributeSet
	 * 前于Preattributechange执行
	 * 一般用于对特性数值的clamp
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	/*
	 * 继承自UAttributeSet
	 * 后于Preattributechange执行
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;

	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockHit, bool bCriticalHit) const;
};






