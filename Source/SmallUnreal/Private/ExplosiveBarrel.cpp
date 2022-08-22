// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SMagicProjectile.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	// Another way to make sure enable simulate physics without use Blueprint.
// 	StaticMeshComp->SetSimulatePhysics(true);
// 	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	RadialForceComp->Radius = 750.0f;
	RadialForceComp->ForceStrength = 2500.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FScriptDelegate delegate;
	delegate.BindUFunction(this, "OnActorHit");
	StaticMeshComp->OnComponentHit.Add(delegate);
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosiveBarrel::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA<ASMagicProjectile>())
	{
		RadialForceComp->FireImpulse();

		UE_LOG(LogTemp, Warning, TEXT("Barrel is exploded!"));
	}
}

