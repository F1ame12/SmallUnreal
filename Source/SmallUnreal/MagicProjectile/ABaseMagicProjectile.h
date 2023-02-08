// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABaseMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UAudioComponent;

UCLASS()
class SMALLUNREAL_API AABaseMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABaseMagicProjectile();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCompOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHitWaveFinished();

	UFUNCTION(BlueprintCallable)
	void SetDamage(float Value);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewSpeed);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void ProcessHitEvent(AActor* OtherActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HitEvent")
	bool IsValidHitActor(AActor* OtherActor);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* MovingAudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* HitAudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue = 20.f;

	FTimerHandle DestoryTimerHandle;
};
