// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacteBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacte.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class RGPSTART_API AAuraCharacte : public AAuraCharacteBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacte();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual int32 GetActorLevel_Implementation() const override;

	/*PlayerInterface*/
	virtual void AddToXP_Implementation(int32 XP) override;
	virtual void AddToLevel_Implementation(int32 AddLevel) override;
	virtual void AddToAttributePoint_Implementation(int32 AttributePoint) override;
	virtual void AddToSpellPoint_Implementation(int32 SpellPoint) override;

	//不处理数据只处理逻辑
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindNewLevelForXP_Implementation(const int32 AllXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 CurLevel) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 CurLevel) const override;
	virtual int32 GetAttributePointCount_Implementation() const override;
	virtual int32 GetSpellPointCount_Implementation() const override;
	/*PlayerInterface End*/
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Niagara")
	TObjectPtr<UNiagaraComponent> Niagara_LevelUpEffect;

private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable) /*需要在所有客户端播放*/
	void MulticastLevelUpEffect() const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

};


