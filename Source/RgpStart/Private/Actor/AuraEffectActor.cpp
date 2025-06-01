// ANNN PRO


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApply)
{
	//如果目标是敌人，但不应用到敌人则返回
	const bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy"));
	if (bIsEnemy && !bApplyEffectToEnemies) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	if (EffectToApply == nullptr) return;

	FGameplayEffectContextHandle Handle = TargetASC->MakeEffectContext();
	Handle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(EffectToApply, EffectLevel, Handle);
	const FActiveGameplayEffectHandle ActiveHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	const bool bIsInfinite = SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolity == EEffectRemovalPolity::OnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveHandle, TargetASC);
	}

	//获取specHandle的duration SpecHandle.Data.Get()->Def.Get()->DurationPolicy
	if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor* OverlappedActor)
{
	if (InstantEffectApplicationPolity== EEffectApplicationPolity::ApplyOnOverlap && InstantEffectToApply != nullptr)
	{
		ApplyEffectToTarget(OverlappedActor, InstantEffectToApply);
	}

	if (DurationEffectApplicationPolity== EEffectApplicationPolity::ApplyOnOverlap && DurationEffectToApply != nullptr)
	{
		ApplyEffectToTarget(OverlappedActor, DurationEffectToApply);
	}

	if (InfiniteEffectApplicationPolity== EEffectApplicationPolity::ApplyOnOverlap && InfiniteEffectToApply != nullptr)
	{
		ApplyEffectToTarget(OverlappedActor, InfiniteEffectToApply);

	}

	
}

void AAuraEffectActor::OnEndOverlap(AActor* OverlappedActor)
{
	if (InstantEffectApplicationPolity== EEffectApplicationPolity::ApplyOnEndOverlap && InstantEffectToApply != nullptr)
	{
		ApplyEffectToTarget(OverlappedActor, InstantEffectToApply);
	}

	if (DurationEffectApplicationPolity== EEffectApplicationPolity::ApplyOnEndOverlap && DurationEffectToApply != nullptr)
	{
		ApplyEffectToTarget(OverlappedActor, DurationEffectToApply);
	}

	if (InfiniteEffectApplicationPolity== EEffectApplicationPolity::ApplyOnEndOverlap && InfiniteEffectToApply != nullptr)
	{
		ApplyEffectToTarget(OverlappedActor, InfiniteEffectToApply);

	}

	if (InfiniteEffectRemovalPolity== EEffectRemovalPolity::OnEndOverlap && InfiniteEffectToApply != nullptr)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlappedActor);
		if (TargetASC == nullptr) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		//remove effect
		for (auto& Pair : ActiveEffectHandles)
		{
			if (TargetASC == Pair.Value)
			{
				Pair.Value->RemoveActiveGameplayEffect(Pair.Key,1);
				HandlesToRemove.Add(Pair.Key);
			}
		}

		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}

	}
	
}



