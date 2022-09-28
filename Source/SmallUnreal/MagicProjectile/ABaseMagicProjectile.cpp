// Fill out your copyright notice in the Description page of Project Settings.


#include "../MagicProjectile/ABaseMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "SAttributeComponent.h"

// Sets default values
AABaseMagicProjectile::AABaseMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USphereComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("Projectile");
	MeshComp->IgnoreActorWhenMoving(GetInstigator(), true);
	RootComponent = MeshComp;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComp");
	MoveComp->InitialSpeed = InitialSpeed;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.0f;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(MeshComp);

	MovingAudioEffect = CreateDefaultSubobject<UAudioComponent>("MovingAudioEffect");
	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>("HitAudioEffect");
}

// Called when the game starts or when spawned
void AABaseMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABaseMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FScriptDelegate HitDelegate;
	HitDelegate.BindUFunction(this, "OnCompHit");
	MeshComp->OnComponentHit.Add(HitDelegate);

	FScriptDelegate OverlapDelegate;
	OverlapDelegate.BindUFunction(this, "OnCompOverlap");
	MeshComp->OnComponentBeginOverlap.Add(OverlapDelegate);

	HitAudioEffect->OnAudioFinished.AddDynamic(this, &AABaseMagicProjectile::OnHitWaveFinished);
}

// Called every frame
void AABaseMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABaseMagicProjectile::OnCompHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 16, FColor::Green, false, 2.0f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint);
	Destroy();
}

void AABaseMagicProjectile::OnCompOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetTransform());
	MovingAudioEffect->Stop();
	HitAudioEffect->Play();

	//MoveComp->bSimulationEnabled = false;

	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
		}
	}
}

void AABaseMagicProjectile::OnHitWaveFinished_Implementation()
{
	Destroy();
}

