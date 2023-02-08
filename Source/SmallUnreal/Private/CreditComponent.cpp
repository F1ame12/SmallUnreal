
#include "CreditComponent.h"
#include "MyPlayerState.h"
#include "GameFramework/Character.h"

UCreditComponent* UCreditComponent::GetCreditComponent(AActor* Actor)
{
	if (Actor)
	{
		return Cast<UCreditComponent>(Actor->GetComponentByClass(UCreditComponent::StaticClass()));
	}

	return nullptr;
}

float UCreditComponent::GetCredits() const
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (Player)
	{
		AMyPlayerState* State = Cast<AMyPlayerState>(Player->GetPlayerState());

		if (State) return State->GetCreditAmount();
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerState is null. Can't get CreditAmount."))

	return 0;
}

float UCreditComponent::ChangeCreditAmount(float Delta)
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (Player)
	{
		AMyPlayerState* State = Cast<AMyPlayerState>(Player->GetPlayerState());

		if (State)
		{
			// 触发积分变化事件
			OnCreditAmountChanged.Broadcast(Delta, State->ChangeCredit(Delta));

			return State->GetCreditAmount();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerState is null. Can't get CreditAmount."))

	return 0;
}
