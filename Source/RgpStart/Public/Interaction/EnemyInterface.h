// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RGPSTART_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighLightActor()=0;
	virtual void UnHighLightActor()=0;

	/*
	 * BlueprintNativeEvent
	 * 保留原本实现的同时可以被蓝图重写
	 * 与 BlueprintImplementableEvent 区别在于 BlueprintNativeEvent 有默认的实现
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTarget(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTarget() const;
};
