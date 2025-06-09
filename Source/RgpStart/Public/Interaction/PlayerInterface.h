// ANNN PRO

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RGPSTART_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToXP(int32 XP);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToLevel(int32 AddLevel);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToAttributePoint(int32 AttributePoint);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToSpellPoint(int32 SpellPoint);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 FindNewLevelForXP(const int32 AllXP) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePointsReward(int32 CurLevel) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSpellPointsReward(int32 CurLevel) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePointCount() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSpellPointCount() const;
};
