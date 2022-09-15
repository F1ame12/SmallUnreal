// Fill out your copyright notice in the Description page of Project Settings.


#include "../MagicProjectile/TeleportProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "ABaseMagicProjectile.h"

ATeleportProjectile::ATeleportProjectile()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HitParticleSystemObj(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld.P_Gideon_Primary_HitWorld'"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EffectParticleSystemObj(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_Projectile.P_Gideon_Primary_Projectile'"));

	HitEffect = HitParticleSystemObj.Object;
	EffectComp->Template = EffectParticleSystemObj.Object;
}

// Called when the game starts or when spawned
void ATeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void ATeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ATeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}