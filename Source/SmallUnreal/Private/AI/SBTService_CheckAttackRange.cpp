// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include <stdexcept>
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/SAICharacter.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());

					bool bWithinRange = DistanceTo < 1000.0f;

					bool bLineOfSight = AIController->LineOfSightTo(TargetActor);

					ASAICharacter* AICharacter = Cast<ASAICharacter>(AIPawn);
					FVector WeaptionLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
					// 确保武器发射不会被阻碍
					bool bShootingBlock = GetWorld()->LineTraceTestByChannel(WeaptionLocation, TargetActor->GetActorLocation(), ECC_Visibility);

					bool bCanAttack = bWithinRange && bLineOfSight && !bShootingBlock;
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bCanAttack);
				}
			}
		}
	}
}
