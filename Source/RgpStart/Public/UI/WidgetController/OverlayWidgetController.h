// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText = FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> WidgetClass = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Icon = nullptr;
	
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature,float,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature,FUIWidgetRow,Row);
/**
 * 
 */


UCLASS(BlueprintType, Blueprintable)
class RGPSTART_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
    FOnAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
    FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
    FOnAttributeChangedSignature OnManaChanged;

    UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
    FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Ability")
	FOnPLayerStateChangedSignature OnLevelChanged;

	//接收到的是当前经验的百分比
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnXPChanged;
	//游戏事件：获取道具等发出后蓝图接收并处理
	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FOnMessageWidgetRowSignature OnMessageWidgetRow;





protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable = nullptr;



	template<typename T>
	T* GetDataFromDataTable(UDataTable* Table,const FGameplayTag& MessageTag) const;
	
	void OnPlayerLevelChanged(int32 NewLevel) const;
	void OnPlayerXPChanged(int32 NewXP);
};



template <typename T>
T* UOverlayWidgetController::GetDataFromDataTable(UDataTable* Table, const FGameplayTag& MessageTag) const
{
	return Table->FindRow<T>(MessageTag.GetTagName(),TEXT(""));
}
