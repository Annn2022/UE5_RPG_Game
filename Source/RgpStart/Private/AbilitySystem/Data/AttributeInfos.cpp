// ANNN PRO


#include "AbilitySystem/Data/AttributeInfos.h"

FAuraAttributeInfo UAttributeInfos::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFind) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributesArray)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}
	
	if (bLogNotFind)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for tag %s not found on Attributes %s ."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
