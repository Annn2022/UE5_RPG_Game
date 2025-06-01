// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacteBase.h"
#include "AuraCharacte.generated.h"

/**
 * 
 */
UCLASS()
class RGPSTART_API AAuraCharacte : public AAuraCharacteBase
{
	GENERATED_BODY()
public:
	AAuraCharacte();
protected:
	virtual void InitAbilityActorInfo() override;
	virtual int32 GetActorLevel() const override;

private:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
};
