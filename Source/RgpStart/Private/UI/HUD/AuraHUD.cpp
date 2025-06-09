// ANNN PRO


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetAuraOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (AuraOverlayWidgetController == nullptr)
	{
		AuraOverlayWidgetController = NewObject<UOverlayWidgetController>(this, AuraOverlayWidgetControllerClass);
		AuraOverlayWidgetController->SetWidgetControllerParams(Params);
		AuraOverlayWidgetController->BindCallbacksToDependencies();
	}
	
	return AuraOverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributemenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(Params);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (AuraSpellMenuWidgetController == nullptr)
	{
		AuraSpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		AuraSpellMenuWidgetController->SetWidgetControllerParams(Params);
		AuraSpellMenuWidgetController->BindCallbacksToDependencies();
	}
	
	return AuraSpellMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (AuraOverlayWidgetClass == nullptr || AuraOverlayWidgetControllerClass == nullptr)
	{
		return;
	}
	checkf(AuraOverlayWidgetClass, TEXT("OverlayWidgetClass is nullptr, please set it in the  BP_AuraHUD"))
	checkf(AuraOverlayWidgetControllerClass, TEXT("AuraOverlayWidgetControllerClass is nullptr, please set it in the  BP_AuraHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AuraOverlayWidgetClass);
	AuraOverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams Params(PC, PS, ASC, AS);
	AuraOverlayWidgetController = GetAuraOverlayWidgetController(Params);

	AuraOverlayWidget->SetWidgetController(AuraOverlayWidgetController);
	AuraOverlayWidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

