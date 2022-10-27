// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SInteractionComponent.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USAttributeComponent;
class AABaseMagicProjectile;

UCLASS()
class SMALLUNREAL_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AABaseMagicProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AABaseMagicProjectile> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AABaseMagicProjectile> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_TeleportPoint;

protected:

	UPROPERTY(VisibleAnyWhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnyWhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnyWhere)
	USInteractionComponent* InteractComp;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	// player base attack skill.
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

	// fire black hole skill.
	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();

	// teleport player to projectile's point
	void TeleportPoint();
	void TeleportPoint_TimeElapsed();

	virtual void FireProjectile(UClass* FireProjectileClass);

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

};
