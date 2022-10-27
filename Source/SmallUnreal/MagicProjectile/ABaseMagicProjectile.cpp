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
	MoveComp->InitialSpeed = Speed;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.0f;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(MeshComp);

	MovingAudioEffect = CreateDefaultSubobject<UAudioComponent>("MovingAudioEffect");
	MovingAudioEffect->SetupAttachment(MeshComp);
	MovingAudioEffect->SetAutoActivate(false);

	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>("HitAudioEffect");
	HitAudioEffect->SetupAttachment(MeshComp);
	HitAudioEffect->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AABaseMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	MovingAudioEffect->Play();

	GetWorld()->GetTimerManager().SetTimer(DestoryTimerHandle, [this]() {
		Destroy();
		}, 6.0f, false);
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
	if (OtherActor != GetInstigator<AActor>() && !OtherActor->IsA(AABaseMagicProjectile::StaticClass()))
	{
		ProcessHitEvent(OtherActor);
	}
}

void AABaseMagicProjectile::OnCompOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetInstigator<AActor>() && !OtherActor->IsA(AABaseMagicProjectile::StaticClass()))
	{
		ProcessHitEvent(OtherActor);
	}
}

void AABaseMagicProjectile::OnHitWaveFinished_Implementation()
{
	Destroy();
}

void AABaseMagicProjectile::SetDamage(float Value)
{
	DamageValue = Value;
}

void AABaseMagicProjectile::SetSpeed(float NewSpeed)
{
	MoveComp->InitialSpeed = NewSpeed;
}

void AABaseMagicProjectile::ProcessHitEvent(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetTransform());
	MovingAudioEffect->Stop();
	HitAudioEffect->Play();

	// 隐藏并忽略碰撞 保证音效能正常播放结束
	// 注：实际销毁操作在音效结束事件触发
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 停止运动模拟
	FHitResult Hit;
	MoveComp->StopSimulating(Hit);

	// 触发爆炸时取消超时销毁定时器
	GetWorld()->GetTimerManager().ClearTimer(DestoryTimerHandle);

	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-DamageValue);
		}
	}
}

