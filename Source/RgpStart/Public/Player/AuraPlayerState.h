// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/PlayerState.h"

#include "AuraPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32/*新的值*/);
/**
 * 
 */
UCLASS()
class RGPSTART_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStateChanged OnXPChanged;
	FOnPlayerStateChanged OnLevelChanged;

	FOnPlayerStateChanged OnAttributePointChanged;
	FOnPlayerStateChanged OnSpellPointChanged;
	
	FORCEINLINE int32 GetPlayerLevel() const {return Level;}
	FORCEINLINE int32 GetPlayerXP() const {return XP;}
	FORCEINLINE int32 GetPlayerSpellPoint() const {return SpellPoint;}
	FORCEINLINE int32 GetAttributePoint() const {return AttributePoint;}

	void AddXP(int32 XPToAdd);
	void SetXP(int32 NewXP);

	void AddLevel(int32 LevelToAdd);
	void SetLevel(int32 NewLevel);

	void AddAttributePoint(int32 AttributePointToAdd);
	void AddSpellPoint(int32 SpellPointToAdd);
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_XP)
	int32 XP = 1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_AttributePoint)
	int32 AttributePoint = 1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_SpellPoint)
	int32 SpellPoint = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldLevel);

	UFUNCTION()
	void OnRep_SpellPoint(int32 OldXP);
};
