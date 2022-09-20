// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFlashHitBox.generated.h"

class UStaticMeshComponent;
class USAttributeComponent;

UCLASS()
class SMALLUNREAL_API ASFlashHitBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFlashHitBox();

protected:

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	USAttributeComponent* AttributeComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable)
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
