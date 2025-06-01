// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class RGPSTART_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	/*
	 *定义要捕获的属性（如 Vigor 属性）及其捕获方式（Snapshot 或 True）。
	 */
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
