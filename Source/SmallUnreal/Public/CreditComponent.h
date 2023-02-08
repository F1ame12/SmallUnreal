// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "CreditComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditAmountChanged, float, Delta, float, NewAmount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMALLUNREAL_API UCreditComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintCallable, Category = "Credit")
	static UCreditComponent* GetCreditComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	float GetCredits() const;

	UFUNCTION(BlueprintCallable)
	float ChangeCreditAmount(float Delta);

public:

	UPROPERTY(BlueprintAssignable, Category = "Credit")
	FOnCreditAmountChanged OnCreditAmountChanged;

protected:
};
