// ANNN PRO


#include "RgpStart/Public/Character/AuraCharacteBase.h"

#include "FAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "RgpStart/RgpStart.h"


// Sets default values
AAuraCharacteBase::AAuraCharacteBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//防止射线检测被遮挡
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	//取消胶囊体的碰撞事件，只由mesh触发
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacteBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AAuraCharacteBase::GetActorLevel() const
{
	
	return 1;
}

FVector AAuraCharacteBase::GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) const
{
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_LeftHand))
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	else if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_RightHand))
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	else if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Weapon))
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	
	return FVector::ZeroVector;
	

	
}

AActor* AAuraCharacteBase::GetAvatar_Implementation()
{
	return this;
}

bool AAuraCharacteBase::IsDead_Implementation() const
{
	return bIsDead;
}

TArray<FTaggedMontage> AAuraCharacteBase::GetAttackMontage_Implementation() const
{
	return AttackMontages;
}

void AAuraCharacteBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MulticastHandleDeath();
}

UNiagaraSystem* AAuraCharacteBase::GetHitEffect_Implementation() const
{
	return HitEffect;
}

void AAuraCharacteBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	EffectDissolve();
	bIsDead = true;
}


void AAuraCharacteBase::InitAbilityActorInfo()
{
}

void AAuraCharacteBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const
{
	check(GetAbilitySystemComponent())
	check(EffectClass)
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacteBase::InitDefaultAttributes() const
{
	//初始化玩家的主属性
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.0f);

	//初始化玩家的次要属性
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.0f);

	//初始化玩家的必要属性
	ApplyEffectToSelf(DefaultVitalAttributes,1.0f);
}


void AAuraCharacteBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AAuraCharacteBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AAuraCharacteBase::AddCharactorAbilities() const
{
	if (!HasAuthority()) return;
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	AuraASC->AddCharacterAbilities(StartupAbilities);
}

UAnimMontage* AAuraCharacteBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacteBase::EffectDissolve()
{
	if (IsValid(DissolveMaterial))
	{
		UMaterialInstanceDynamic* DissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterial, this);
		GetMesh()->SetMaterial(0, DissolveMaterialInstance);

		StartDissolveTimeline(DissolveMaterialInstance);
	}
	if (IsValid(DissolveMaterial_Weapon))
	{
		UMaterialInstanceDynamic* DissolveMaterialInstance_Weapon = UMaterialInstanceDynamic::Create(DissolveMaterial_Weapon, this);
		Weapon->SetMaterial(0, DissolveMaterialInstance_Weapon);
		StartWeaponDissolveTimeline(DissolveMaterialInstance_Weapon);
	}


}









