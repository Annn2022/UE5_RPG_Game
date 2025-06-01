// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AuraAIController.generated.h"

/**
 * 
 */
UCLASS()
class RGPSTART_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAuraAIController();
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
