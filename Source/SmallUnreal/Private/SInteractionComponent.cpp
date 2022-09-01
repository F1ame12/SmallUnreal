// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGamePlayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End;
	End = EyeLocation + (EyeRotation.Vector() * 1000);

	/* LineTrace Method to check interaction.

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<USGamePlayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}
	FColor DrawColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), EyeLocation, End, DrawColor, false, 2.0f, 0, 2.0f);

	*/
	
	float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByChannel(Hits, EyeLocation, End, FQuat::Identity, ECC_WorldDynamic, Shape);

	FColor DrawColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, DrawColor, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, DrawColor, false, 2.0f, 0, 2.0f);
}