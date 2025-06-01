// ANNN PRO


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFind) const
{
	for (const auto& Action : AllInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFind)
	{
		UE_LOG(LogTemp, Error, TEXT("UAuraInputConfig::FindAbilityInputActionForTag: %s not found"), *InputTag.ToString())
	}
	return nullptr;
}
