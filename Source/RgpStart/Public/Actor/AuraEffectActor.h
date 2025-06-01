// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolity : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolity : uint8
{
	OnEndOverlap,
	DoNotApply
};

UCLASS()
class RGPSTART_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles = {};
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApply);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
	TSubclassOf<UGameplayEffect> InstantEffectToApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
	EEffectApplicationPolity InstantEffectApplicationPolity;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
	TSubclassOf<UGameplayEffect> DurationEffectToApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
    EEffectApplicationPolity DurationEffectApplicationPolity;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
	TSubclassOf<UGameplayEffect> InfiniteEffectToApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
    EEffectApplicationPolity InfiniteEffectApplicationPolity;

    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
    EEffectRemovalPolity InfiniteEffectRemovalPolity;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
    bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
    bool bApplyEffectToEnemies = false;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* OverlappedActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* OverlappedActor);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AuraEffects")
	float EffectLevel = 1.0f;
	
};
