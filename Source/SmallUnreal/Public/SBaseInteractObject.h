// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseInteractObject.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class SMALLUNREAL_API ASBaseInteractObject : public AActor
{
	GENERATED_BODY()
	
public:
	ASBaseInteractObject();

	void PostInitializeComponents() override;

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void OnIntract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	bool Intraction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void InteractCD_TimeElasped();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* BaseMeshComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	USphereComponent* IntractSphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bCanInteract;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	bool bCanRefresh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	float InteractCD;

};
