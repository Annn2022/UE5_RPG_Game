// ANNN PRO


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddXP(int32 XPToAdd)
{
	XP+=XPToAdd;
	OnXPChanged.Broadcast(XP);
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChanged.Broadcast(XP);

}

void AAuraPlayerState::AddLevel(int32 LevelToAdd)
{
	Level+=LevelToAdd;
	OnLevelChanged.Broadcast(Level);

}

void AAuraPlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level);
}

void AAuraPlayerState::AddAttributePoint(int32 AttributePointToAdd)
{
	AttributePoint+=AttributePointToAdd;
	OnAttributePointChanged.Broadcast(AttributePoint);
}

void AAuraPlayerState::AddSpellPoint(int32 SpellPointToAdd)
{
	SpellPoint+=SpellPointToAdd;
	OnSpellPointChanged.Broadcast(SpellPoint);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level);

}

//客户端广播
void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChanged.Broadcast(XP);

}

void AAuraPlayerState::OnRep_AttributePoint(int32 OldLevel)
{
	OnAttributePointChanged.Broadcast(AttributePoint);

}

void AAuraPlayerState::OnRep_SpellPoint(int32 OldXP)
{
	OnSpellPointChanged.Broadcast(SpellPoint);

}


