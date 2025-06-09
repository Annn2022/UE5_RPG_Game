// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class RGPSTART_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> AuraOverlayWidget;

	UOverlayWidgetController* GetAuraOverlayWidgetController(const FWidgetControllerParams& Params) ;
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& Params) ;
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& Params) ;

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> AuraOverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> AuraOverlayWidgetController = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> AuraOverlayWidgetControllerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributemenuWidgetControllerClass;
	
	TObjectPtr<USpellMenuWidgetController> AuraSpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
