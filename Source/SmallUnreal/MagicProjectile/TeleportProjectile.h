// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MagicProjectile/ABaseMagicProjectile.h"
#include "TeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SMALLUNREAL_API ATeleportProjectile : public AABaseMagicProjectile
{
	GENERATED_BODY()
	
public:
	ATeleportProjectile();

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
};
