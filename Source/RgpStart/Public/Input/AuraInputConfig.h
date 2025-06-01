// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;
	
};
/**
 * 
 */
UCLASS()
class RGPSTART_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFind) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraInputConfig")
	TArray<FAuraInputAction> AllInputActions;
};
