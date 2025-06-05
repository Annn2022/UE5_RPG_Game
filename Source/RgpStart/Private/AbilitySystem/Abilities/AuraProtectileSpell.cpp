// ANNN PRO


#include "AbilitySystem/Abilities/AuraProtectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FAuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProtectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	
	if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		// 获取施法者位置 BlueprintNativeEvent 下的调用方式
		const FVector Location = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation = (ProjectileTargetLocation - Location).Rotation();
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		FGameplayEffectContextHandle DamageEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		DamageEffectContextHandle.SetAbility(this);
		DamageEffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		DamageEffectContextHandle.AddActors(Actors);
		Actors.Add(Projectile);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		DamageEffectContextHandle.AddHitResult(HitResult);
		
		//获取影响效果的句柄
		const FGameplayEffectSpecHandle DamageEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), DamageEffectContextHandle);
		FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();

		for (auto& Pairs : DamageTypes)
		{
			const float ScalableDamage = Pairs.Value.GetValueAtLevel(GetAbilityLevel());
			//Spec.GetSetByCallerMagnitude(Tag) 可以取到ScalableDamage
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, Pairs.Key, ScalableDamage);
		}
		
		//将影响效果的Spec传递给子弹（传递伤害
		Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
