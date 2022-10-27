// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

/**
 * 
 */
UCLASS()
class SMALLUNREAL_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ASGameModeBase();

protected:

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	float BotSpawnInterval;

	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	UEnvQuery* BotSpawnPosQuery;

	UPROPERTY(EditDefaultsOnly, Category = "BotSpawn")
	UCurveFloat* DifficultyCurve;

protected:

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void SpawnBotOnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	void StartPlay() override;

};
