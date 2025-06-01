// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacteBase.generated.h"



UCLASS(Abstract)
class RGPSTART_API AAuraCharacteBase : public ACharacter ,public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacteBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	/*ICombat Interface*/
	virtual int32 GetActorLevel() const override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) const override;
	virtual  AActor* GetAvatar_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual  TArray<FTaggedMontage> GetAttackMontage_Implementation() const override;
	virtual void Die() override;
	virtual UNiagaraSystem* GetHitEffect_Implementation() const override;
	/*ICombat Interface*/
	//client
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere,Category= "战斗")
	TArray<FTaggedMontage> AttackMontages;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "战斗")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere,Category= "战斗")
	FName WeaponTipSocketName = "TipSocket";

	UPROPERTY(EditAnywhere,Category= "战斗")
	FName RightHandSocketName = "RightHandSocket";

	UPROPERTY(EditAnywhere,Category= "战斗")
	FName LeftHandSocketName = "LeftHandSocket";

	bool bIsDead = false;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "A_角色属性")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "A_角色属性")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "A_角色属性")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float Level) const;
	virtual  void InitDefaultAttributes() const;

	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void AddCharactorAbilities() const;

	virtual  UAnimMontage* GetHitReactMontage_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "A_角色资源")
	TObjectPtr<UMaterialInstance> DissolveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "A_角色资源")
	TObjectPtr<UMaterialInstance> DissolveMaterial_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "A_角色资源")
	UNiagaraSystem* HitEffect;

	void EffectDissolve();

	UFUNCTION(BlueprintImplementableEvent, Category = "注册函数")
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "注册函数")
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstance);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "A_角色参数")
	float DissolveTime = 1.f;
private:

	UPROPERTY(EditAnywhere, Category = "A_初始能力集")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "A_角色能力集")
	UAnimMontage* HitReactMontage;
};
