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


	void OnCompHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	FTimerHandle TeleportEndTimer;
	FTimerHandle TeleportTimer;

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void OnHitTeleport(APawn* Player);
	virtual void OnHitTeleport_TimeElpsed(APawn* Player);
};
