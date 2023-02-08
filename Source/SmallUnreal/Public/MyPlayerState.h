// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"


UCLASS()
class SMALLUNREAL_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Credit")
	float ChangeCredit(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Credit")
	float GetCreditAmount() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credit")
	float CreditAmount;

};
