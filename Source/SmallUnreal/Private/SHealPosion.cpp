// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealPosion.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASHealPosion::ASHealPosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	PosionMesh = CreateDefaultSubobject<UStaticMeshComponent>("PosionMesh");
	PosionMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ASHealPosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHealPosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASHealPosion::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

