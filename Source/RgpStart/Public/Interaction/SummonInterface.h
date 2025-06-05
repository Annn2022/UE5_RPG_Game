// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SummonInterface.generated.h"

// 召唤角色继承的接口

UINTERFACE(MinimalAPI)
class USummonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RGPSTART_API ISummonInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetMinionCount() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsMaxMinion() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddMinionCount(int32 Amount);

};
