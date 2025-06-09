// ANNN PRO


#include "Character/AuraCharacte.h"

#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "opensubdiv/vtr/level.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


class AAuraPlayerState;

AAuraCharacte::AAuraCharacte()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	Niagara_LevelUpEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara_LevelUpEffect"));
	Niagara_LevelUpEffect->SetupAttachment(GetRootComponent());
	Niagara_LevelUpEffect->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Elementalist;
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



int32 AAuraCharacte::GetActorLevel_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	return AuraPlayerState->GetPlayerLevel();
	
}

void AAuraCharacte::AddToXP_Implementation(const int32 XP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	AuraPlayerState->AddXP(XP);

}

inline void AAuraCharacte::AddToLevel_Implementation(int32 AddLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	AuraPlayerState->AddLevel(AddLevel);
	//ApplyEffectToSelf(DefaultSecondaryAttributes,AuraPlayerState->GetPlayerLevel());
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacte::AddToAttributePoint_Implementation(int32 AttributePoint)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	AuraPlayerState->AddAttributePoint(AttributePoint);
}

void AAuraCharacte::AddToSpellPoint_Implementation(int32 SpellPoint)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	AuraPlayerState->AddSpellPoint(SpellPoint);
}

void AAuraCharacte::LevelUp_Implementation()
{
	MulticastLevelUpEffect();
}

void AAuraCharacte::MulticastLevelUpEffect_Implementation() const
{
	if( IsValid(Niagara_LevelUpEffect))
	{
		//特效需要面向相机播放
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector PlayerLocation = GetActorLocation();
		
		const FRotator Rotation = (CameraLocation - PlayerLocation).Rotation();
		Niagara_LevelUpEffect->SetWorldRotation(Rotation);
		Niagara_LevelUpEffect->Activate(true);

	}
}



int32 AAuraCharacte::GetXP_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	return AuraPlayerState->GetPlayerXP();
	
}

int32 AAuraCharacte::FindNewLevelForXP_Implementation(const int32 AllXP) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(AllXP);
	
}

int32 AAuraCharacte::GetAttributePointsReward_Implementation(int32 CurLevel) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[CurLevel].AttributePointAward;
	return 0;
}

int32 AAuraCharacte::GetSpellPointsReward_Implementation(int32 CurLevel) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[CurLevel].SpellPointAward;
}

int32 AAuraCharacte::GetAttributePointCount_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoint();
}

int32 AAuraCharacte::GetSpellPointCount_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerSpellPoint();
}
