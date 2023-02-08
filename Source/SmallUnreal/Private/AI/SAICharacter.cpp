// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "CreditComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttrComp = CreateDefaultSubobject<USAttributeComponent>("AttrComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SenseComp = CreateDefaultSubobject<UPawnSensingComponent>("SenseComp");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SenseComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttrComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* Target)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsObject("TargetActor", Target);

			//UE_LOG(LogTemp, Log, TEXT("AI %s find player!"), *GetName());
		}
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f && NewHealth <= 0.0f)
	{
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->GetBrainComponent()->StopLogic("Killed");

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("RagDoll");
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

			SetLifeSpan(10.0f);
		}
	}
	else
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
	}
}
