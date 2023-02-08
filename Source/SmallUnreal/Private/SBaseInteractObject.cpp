// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseInteractObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


ASBaseInteractObject::ASBaseInteractObject()
{
	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComp");
	BaseMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	BaseMeshComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	RootComponent = BaseMeshComp;

	IntractSphere = CreateDefaultSubobject<USphereComponent>("IntractSphere");
	IntractSphere->SetGenerateOverlapEvents(true);
	IntractSphere->SetCollisionObjectType(ECC_WorldDynamic);
	IntractSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	IntractSphere->SetupAttachment(BaseMeshComp);

	bCanInteract = true;
	bCanRefresh = true;
	InteractCD = 10.0;
}

void ASBaseInteractObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	IntractSphere->OnComponentBeginOverlap.AddDynamic(this, &ASBaseInteractObject::OnIntract);
}

void ASBaseInteractObject::OnIntract_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanInteract) return;

	if (Intraction(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult))
	{
		bCanInteract = false;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		if (bCanRefresh)
		{
			FTimerHandle RefreshTimerHandle;
			GetWorldTimerManager().SetTimer(RefreshTimerHandle, this, &ASBaseInteractObject::InteractCD_TimeElasped, InteractCD);
		}
		else
		{
			Destroy();
		}
	}
}

bool ASBaseInteractObject::Intraction_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Intraction implementation is empty."))

	return false;
}

void ASBaseInteractObject::InteractCD_TimeElasped()
{
	bCanInteract = true;

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

