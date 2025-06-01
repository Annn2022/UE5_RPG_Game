// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfos.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	float BaseValue = 0.f;
};

/**
 * 
 */
UCLASS()
class RGPSTART_API UAttributeInfos : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFind = false) const;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraAttributeInfo> AttributesArray = {};
	
};
