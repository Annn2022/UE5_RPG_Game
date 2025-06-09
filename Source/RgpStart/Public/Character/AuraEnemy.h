// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacteBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"
class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class RGPSTART_API AAuraEnemy : public AAuraCharacteBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	virtual int32 GetActorLevel_Implementation() const override;
	virtual void Die() override;

	//
	virtual void SetCombatTarget_Implementation(AActor* NewTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	void HitReactTahChanged(const FGameplayTag EventTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly)
	bool bHighLighted;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Defaults")
	bool bIsHitReacting;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Enemy Defaults")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Defaults")
	float DeathTimeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Defaults")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitDefaultAttributes() const override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Enemy Defaults")
	int32 Level = 1;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Defaults")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Enemy Defaults")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Enemy Defaults")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
};
