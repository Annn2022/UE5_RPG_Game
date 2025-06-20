// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RGPSTART_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPLayerStateChangedSignature OnSpellPointChangedDelegate;

};
