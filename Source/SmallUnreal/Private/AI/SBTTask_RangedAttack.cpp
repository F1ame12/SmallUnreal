// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");

		UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
		if (BlackBoardComp == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetPlayer = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetPlayer == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector TargetLocation = TargetPlayer->GetActorLocation();

		FRotator TargetDirection = (TargetLocation - MuzzleLocation).Rotation();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, TargetDirection, Params);
	}

	return EBTNodeResult::Failed;
}
