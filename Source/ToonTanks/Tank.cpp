// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"


ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera= CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Respond when the WASD keys are pressed
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    //Respond when the shitft key is pressed or released
    PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &ATank::StartDodge);
    PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Released, this, &ATank::EndDodge);

    //Respond when the "fire" and "shield" key is pressed or released
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::CanShoot);
    PlayerInputComponent->BindAction(TEXT("Shield"), IE_Pressed, this, &ATank::Block);

    //Respond when shield key is released
    PlayerInputComponent->BindAction(TEXT("Shield"), IE_Released, this, &ATank::UnBlock);
    //PlayerInputComponent->BindAction(TEXT("ShieldRelease"), IE_Released, this, &ATank::Block);
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, 
        false,
        HitResult);
        
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    //X = Value * DeltaTime * Speed
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    //Yaw = Value * TurnRate * DeltaTime
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::StartDodge()
{
    //Timer calls end dodge after spawn dealy (.15 seconds) if the shield is active
    if (IsDodgeActive)
    {
        Speed = 2600;
        //IsDodgeActive = false;
        GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &ATank::EndDodge, DodgeSpawnDelay, false);  
    }
}
//The dodge is ended and the respawn timer is set for 5 seconds before you can dodge again
void ATank::EndDodge()
{
    Speed = 800;
    if (IsDodgeActive)
    {
        IsDodgeActive = false;
        GetWorld()->GetTimerManager().SetTimer(DodgeRespawnHandle, this, &ATank::SetDodge, DodgeActiveDelay, false);
    }
    
}

void ATank::CanShoot()
{
    //if the player has ammo, call Fire() from base pawn
    if (CurrentAmmo > 0)
    {
        Fire();
        CurrentAmmo--;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Out of ammo!"));
    }
}

void ATank::AddAmmo()
{
    //if the shield makes a successful block, addammo is called from the adddynamic delegate on theshield class
    CurrentAmmo += 5;
}

void ATank::SetDodge()
{
    UE_LOG(LogTemp, Display, TEXT("Set Dodge"));
    IsDodgeActive = true;
}

bool ATank::GetDodgeUI()
{
    return IsDodgeActive;
}

float ATank::GetAmmoUI()
{
    return CurrentAmmo;
}
   
