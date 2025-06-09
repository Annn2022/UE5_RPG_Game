// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfos;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RGPSTART_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|WidgetController")
	FOnPLayerStateChangedSignature OnAttributePointChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|WidgetController")
	FOnPLayerStateChangedSignature OnSpellPointChangedDelegate;

	UFUNCTION(BlueprintCallable, Category = "GAS|WidgetController")
	bool AddAttributeValue(const FGameplayTag& AttributeTag, const int32 AddNum = 1);
protected:
	//属性集资产
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfos> MyAttributeInfo;
};
