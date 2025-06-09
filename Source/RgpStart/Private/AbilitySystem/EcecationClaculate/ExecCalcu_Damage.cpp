// ANNN PRO


#include "AbilitySystem/EcecationClaculate/ExecCalcu_Damage.h"

#include "AuraAbilityTypes.h"
#include "FAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraGASLibrary.h"
#include "Interaction/CombatInterface.h"

class UCharacterClassInfo;

struct AuraDamageStatics
{
	//声明需要捕获的属性 自动生成 FGameplayEffectAttributeCaptureDefinition 的变量
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritcalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritcalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritcalHitDamage);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptueDefs;
	
	AuraDamageStatics()
	{
		//定义属性的捕获规则 1.属性集 2.目标属性 3.SourceORTarget 4.是否快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritcalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritcalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritcalHitDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
		
		
	}

public:
	void Init()
	{
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptueDefs.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Secondary_CritcalHitChance, CritcalHitChanceDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Secondary_CritcalHitDamage, CritcalHitDamageDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Secondary_CritcalHitResistance, CritcalHitResistanceDef);
		
		TagsToCaptueDefs.Add(Tags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptueDefs.Add(Tags.Attribute_Resistance_Physical,PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	if (DStatics.TagsToCaptueDefs.Num() <= 1)
	{
		DStatics.TagsToCaptueDefs.Empty();
		DStatics.Init();
	}
	return DStatics;
	
}

UExecCalcu_Damage::UExecCalcu_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritcalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritcalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritcalHitDamageDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalcu_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CritcalHitChance, DamageStatics().CritcalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CritcalHitResistance, DamageStatics().CritcalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CritcalHitDamage, DamageStatics().CritcalHitDamageDef);

	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetActorLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		 TargetPlayerLevel = ICombatInterface::Execute_GetActorLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0.f;
	for (const auto& Pairs : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageType = Pairs.Key;
		const FGameplayTag Resistance = Pairs.Value;

		checkf(DamageStatics().TagsToCaptueDefs.Contains(Resistance), TEXT("Missing attribute capture definition for %s"), *Resistance.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = DamageStatics().TagsToCaptueDefs[Resistance];

		float res = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, res);
		res = FMath::Clamp<float>(res, 0.f, 100.f);
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType, false);
		DamageTypeValue *= (100.f-res)/100.f;
		
		Damage += DamageTypeValue;
	}	
	//1.先计算blockChance 是否触发 伤害减半
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
	BlockChance = FMath::Max<float>(0.f, BlockChance);

	bool bBlock = FMath::RandRange(1.f, 100.f) < BlockChance;
	
	UAuraGASLibrary::SetIsBlockedHit(EffectContextHandle, bBlock);

	Damage= bBlock ? Damage / 2.f : Damage;

	//获取到目标的护甲值
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	//来源的穿甲值
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	//不同等级下有不同的穿甲计算系数 可用CT配置
	const UCharacterClassInfo* CharacterClassInfo = UAuraGASLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	//不同等级下有不同的护甲系数 可用CT配置
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	
	// 计算守护甲影响后的伤害
	Damage *= ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;
	
	//来源的暴击几率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritcalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	//目标的暴击抗性
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritcalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	//来源的暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritcalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	//随等级变化的暴击抗性系数
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	// 计算是否暴击
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	
	UAuraGASLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// 暴击伤害计算
	Damage = bCriticalHit ? (1.5f+ SourceCriticalHitDamage/100.f)* Damage : Damage;
	
	//设置最终伤害
	const FGameplayModifierEvaluatedData EveluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EveluatedData);
}
