// Fill out your copyright notice in the Description page of Project Settings.


#include "../MagicProjectile/ABaseMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AABaseMagicProjectile::AABaseMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USphereComponent>("MeshComp");
	RootComponent = MeshComp;
	MeshComp->SetCollisionProfileName("Projectile");
	MeshComp->IgnoreActorWhenMoving(GetOwner(), true);
	MeshComp->SetSimulatePhysics(true);

	FScriptDelegate HitDelegate;
	HitDelegate.BindUFunction(this, "OnCompHit_Implementation");
	MeshComp->OnComponentHit.Add(HitDelegate);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComp");
	MoveComp->InitialSpeed = 1000.0f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.0f;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(MeshComp);

	Super::SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

// Called when the game starts or when spawned
void AABaseMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABaseMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABaseMagicProjectile::OnCompHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 2.0f, 16, FColor::Green, false, 2.0f);
	Destroy();
}

