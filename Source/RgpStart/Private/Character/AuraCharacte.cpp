// ANNN PRO


#include "Character/AuraCharacte.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


class AAuraPlayerState;

AAuraCharacte::AAuraCharacte()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacte::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//服务器上初始化Ability 
	InitAbilityActorInfo();

	AddCharactorAbilities();
}

void AAuraCharacte::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//本地客户端初始化
	InitAbilityActorInfo();
	

}

/**
 * 初始化playerstate上的abilitycomponet数据
 * 初始化pawn上的asc和as
 */
void AAuraCharacte::InitAbilityActorInfo()
{
	if (AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())//过早调用会取不到值
	{
		AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
		AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
		AttributeSet = AuraPlayerState->GetAttributeSet();
		Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
		if (AAuraPlayerController* AuraPlayerController= Cast<AAuraPlayerController>(GetController()))
		{
			if (AAuraHUD* auraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
				auraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}

	InitDefaultAttributes();
}

int32 AAuraCharacte::GetActorLevel() const
{
	if (AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())//过早调用会取不到值
	{
		return AuraPlayerState->GetPlayerLevel();
	}
	return 1;
}
