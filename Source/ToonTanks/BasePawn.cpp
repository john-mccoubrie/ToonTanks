// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "TheShield.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	ShieldSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Shield Spawn Point"));
	ShieldSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles && DeathSound && DeathCameraShakeClass)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}	
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 10.f));

}

void ABasePawn::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);
}

void ABasePawn::Block()
{
	if (IsShieldActive)
	{
		//Gets the shield locaiton and rotation for the spawn point
		FVector Location = ShieldSpawnPoint->GetComponentLocation();
		FRotator Rotation = ShieldSpawnPoint->GetComponentRotation();

		//Attaches the shield to the turret mesh component
		AActor* Shield = GetWorld()->SpawnActor<ATheShield>(ShieldClass, Location, Rotation);
		Shield->AttachToComponent(TurretMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
		//calls unblock after shieldspawndelay of 0.15 seconds
		//GetWorld()->GetTimerManager().SetTimer(ShieldTimerHandle, this, &ABasePawn::UnBlock, ShieldSpawnDelay, false);
	}	
}

void ABasePawn::UnBlock()
{
	//find a reference to all shield actors in the game
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATheShield::StaticClass(), ActorsToFind);
	for (AActor* ShieldActor: ActorsToFind)
	{
		//Destroy the shield actor
		ShieldActor->Destroy();
		//no shield
		IsShieldActive = false;
		//delay when unblocked
		GetWorld()->GetTimerManager().SetTimer(ShieldTimerHandle, this, &ABasePawn::SetShield, ShieldActiveDelay, false);
	}
}

void ABasePawn::SetShield()
{
	//UE_LOG(LogTemp, Display, TEXT("Set Shield"));
	IsShieldActive = true;
}

bool ABasePawn::GetShieldUI()
{
	return IsShieldActive;
}
