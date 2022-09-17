// Fill out your copyright notice in the Description page of Project Settings.


#include "../MagicProjectile/TeleportProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "ABaseMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

ATeleportProjectile::ATeleportProjectile()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HitParticleSystemObj(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld.P_Gideon_Primary_HitWorld'"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EffectParticleSystemObj(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_Projectile.P_Gideon_Primary_Projectile'"));

	HitEffect = HitParticleSystemObj.Object;
	EffectComp->Template = EffectParticleSystemObj.Object;
}

void ATeleportProjectile::OnCompHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 16, FColor::Green, false, 2.0f);

	OnHitTeleport(GetInstigator());
}

// Called when the game starts or when spawned
void ATeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TimerDele = FTimerDelegate::CreateUObject(this, &ATeleportProjectile::OnHitTeleport, GetInstigator());
	GetWorldTimerManager().SetTimer(TeleportEndTimer, TimerDele, 0.2f, false);
}

void ATeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATeleportProjectile::OnHitTeleport(APawn* Player)
{
	GetWorldTimerManager().ClearTimer(TeleportEndTimer);

	FTimerDelegate TimerDele = FTimerDelegate::CreateUObject(this, &ATeleportProjectile::OnHitTeleport_TimeElpsed, Player);
	GetWorldTimerManager().SetTimer(TeleportTimer, TimerDele, 0.2f, false);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetTransform());
}

void ATeleportProjectile::OnHitTeleport_TimeElpsed(APawn* Player)
{
	if (ensure(Player))
	{
		Player->SetActorLocation(GetTransform().GetLocation(), false);
	}

	Destroy();
}

// Called every frame
void ATeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}