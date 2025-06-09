// ANNN PRO


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerState.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//控制属性计算的上下文参数（如是否忽略标签限制、是否应用系数等）
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDef, Spec,EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(0,Vigor);
	
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		const int32 Level = ICombatInterface::Execute_GetActorLevel(Spec.GetContext().GetSourceObject());
		return 80+2*Vigor + 10*Level;

	}
	return 0;
}
