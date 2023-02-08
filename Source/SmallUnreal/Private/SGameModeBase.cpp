// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "DrawDebugHelpers.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "CreditComponent.h"

ASGameModeBase::ASGameModeBase()
{
	BotSpawnInterval = 0.2f;
	BotKillCredit = 10;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, BotSpawnInterval, true);
}

void ASGameModeBase::ControlSpawnBot(bool bShouldPause)
{
	if (bShouldPause)
	{
		GetWorldTimerManager().PauseTimer(TimerHandle_SpawnBots);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(TimerHandle_SpawnBots);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, BotSpawnPosQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::SpawnBotOnQueryComplete);
	}
}

void ASGameModeBase::SpawnBotOnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QueryInstance is null."))
		return;
	}

	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("QueryStatus is not success."))
		return;
	}

	
	float NowBotTotal = 0.0f;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		if (Bot == nullptr) continue;

		USAttributeComponent* BotAttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (BotAttributeComp && BotAttributeComp->IsAlive())
		{
			NowBotTotal++;
		}
	}

	float BotSpawnLimit = 15.0f;
	if (ensure(DifficultyCurve))
	{
		BotSpawnLimit = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
		DifficultyCurve->GetCurves();
	}

	if (NowBotTotal >= BotSpawnLimit) return;

	TArray<FVector> Locations;
	bool bHasResult = QueryInstance->GetQueryResultsAsLocations(Locations);
	if (bHasResult && Locations.Num() > 0)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* NewBot = GetWorld()->SpawnActor<AActor>(BotClass, Locations[0], FRotator::ZeroRotator, Params);

		DrawDebugSphere(GetWorld(), Locations[0], 6.0f, 8, FColor::Green, true);
	}
}

void ASGameModeBase::OnActorKilled(AActor* KilledActor, AActor* Killer)
{
	if (KilledActor && KilledActor->IsA(ASAICharacter::StaticClass()))
	{
		if (Killer && Killer->IsA(ASCharacter::StaticClass()))
		{
			ASCharacter* Player = Cast<ASCharacter>(Killer);
			

			UCreditComponent* CreditComp = UCreditComponent::GetCreditComponent(Killer);
			if (CreditComp)
			{
				CreditComp->ChangeCreditAmount(BotKillCredit);
			}
		}
	}
}
