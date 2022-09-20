// Fill out your copyright notice in the Description page of Project Settings.


#include "SFlashHitBox.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASFlashHitBox::ASFlashHitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
	RootComponent = CubeMesh;

	CubeMesh->SetCollisionObjectType(ECC_WorldDynamic);
	//CubeMesh->SetCollisionResponseToChannels()

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	FScriptDelegate OnHealthChangedDelegate;
	OnHealthChangedDelegate.BindUFunction(this, "OnHealthChanged");
	AttributeComp->OnHealthChanged.Add(OnHealthChangedDelegate);

	//CubeMesh->SetScalarParameterValueOnMaterials("", AttributeComp->GetHealth());
}

// Called when the game starts or when spawned
void ASFlashHitBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASFlashHitBox::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	CubeMesh->SetScalarParameterValueOnMaterials("NewHealth", NewHealth / AttributeComp->GetMaxHealth());

	if (Delta < 0.0f)
	{
		CubeMesh->SetScalarParameterValueOnMaterials("GameTime", GetWorld()->TimeSeconds);
	}

}

// Called every frame
void ASFlashHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

