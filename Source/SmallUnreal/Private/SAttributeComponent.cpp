// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SGameModeBase.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxMealth = 100.0f;

	Health = MaxMealth;
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* Actor)
{
	if (Actor)
	{
		return Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	// 已经死亡 无法继续承受伤害
	if (Health <= 0 && Delta < 0) return false;

	Health += Delta;
	Health = FMath::Clamp<float>(Health, 0, MaxMealth);
	
	if (Health <= 0)
	{
		ASGameModeBase* GameMode = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is null."))
		}
		
	}

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

