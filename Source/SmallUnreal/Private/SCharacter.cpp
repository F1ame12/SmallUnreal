// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "../MagicProjectile/ABaseMagicProjectile.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "CreditComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("NoiseEmitterComp");

	InteractComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	CreditComp = CreateDefaultSubobject<UCreditComponent>("CreditComp");
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("TeleportPoint", IE_Pressed, this, &ASCharacter::TeleportPoint);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Roll = 0.0f;
	ControlRotator.Pitch = 0.0f;

	AddMovementInput(ControlRotator.Vector(), Value);
} 

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Roll = 0.0f;
	ControlRotator.Pitch = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)
	FVector RightVector = FRotationMatrix(ControlRotator).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);

	MakeNoise(1.0f, this, GetActorLocation(), 2.0f);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FireProjectile(ProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
	InteractComp->PrimaryInteract();
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	FireProjectile(BlackHoleProjectileClass);
}

void ASCharacter::TeleportPoint()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportPoint, this, &ASCharacter::TeleportPoint_TimeElapsed, 0.2f);
}

void ASCharacter::TeleportPoint_TimeElapsed()
{
	FireProjectile(TeleportProjectileClass);
}

void ASCharacter::FireProjectile(UClass* FireProjectileClass)
{
	if (!ensure(FireProjectileClass))
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::Red, TEXT("FireProjectileClass is Null!"));
		return;
	}

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	// try to find the crossshair location and direction that in the game world.
	FVector CrossShairWorldLocation;
	FVector CrossShairWorldDirection;
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		FVector2D CrossShairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
		UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			CrossShairLocation, CrossShairWorldLocation, CrossShairWorldDirection);
	}

	FVector LineTraceEnd = CrossShairWorldLocation + (CrossShairWorldDirection * 1000);

	//DrawDebugLine(GetWorld(), HandLocation, LineTraceEnd, FColor::Blue, false, 5.0f, 0, 2.0f);

	FHitResult TargetHit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(TargetHit, HandLocation, LineTraceEnd, ObjectQueryParams);


	FVector TargetLocation = bBlockingHit ? TargetHit.ImpactPoint : LineTraceEnd;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);

	FTransform SpawnTM = FTransform(TargetRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(FireProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials("HitTime", GetWorld()->TimeSeconds);
}

