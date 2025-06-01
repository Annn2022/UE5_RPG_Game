// ANNN PRO


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* OwnerPawn = AIOwner->GetPawn();

	/*
	 *判断OwnerPawn是否是Player或其他类型  运用了Tag
	 * 如果自己是player 则寻找enemy；如果是enemy 则寻找player
	 */
	const FName TargetTag = OwnerPawn->ActorHasTag(FName("Player"))? FName("Enemy"): FName("Player");

	//获取所有带有TargetTag的Actor（OwnerPawn所在的world中寻找
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwnerPawn, TargetTag, ActorsWithTag);

	AActor* NearestActor = nullptr;
	float NearestDistance = FLT_MAX;
	//float NearestDistance = TNumericLimits<float>::Max();
	for (auto Target : ActorsWithTag)
	{
		//float Distance = FVector::DistSquared(OwnerPawn->GetActorLocation(), Actor->GetActorLocation());
		float Distance = OwnerPawn->GetDistanceTo(Target);
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestActor = Target;
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, NearestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, NearestDistance);
}
