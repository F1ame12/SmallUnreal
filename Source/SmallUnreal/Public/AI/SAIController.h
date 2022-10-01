// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class SMALLUNREAL_API ASAIController : public AAIController
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

protected:

	void BeginPlay() override;
};
