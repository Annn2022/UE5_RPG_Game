// ANNN PRO


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//本地控制上传服务器
		SendMouseCursorData();
	}
	else
	{
		//绑定委托 等待数据
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey())
		.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//如果数据到了则调用，否则等待
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey());

		if (!bCalledDelegate)
		{ //数据没到时 等待数据到
			SetWaitingOnRemotePlayerData();
		}
	}
	
	// APlayerController* PlayerController =  Ability->GetActorInfo().PlayerController.Get();
	// FHitResult HitResult;
	// PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	// ValidData.Broadcast(HitResult.Location);
}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	// 预测作用域
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get());
	
	APlayerController* PlayerController =  Ability->GetActorInfo().PlayerController.Get();
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);

	FGameplayAbilityTargetDataHandle Handle;
	FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit();
	HitData->HitResult = HitResult;
	Handle.Add(HitData);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		Handle,
		FGameplayTag(),	
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(Handle);
	}
}

//等待数据到了再执行
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//清除数据缓存
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
