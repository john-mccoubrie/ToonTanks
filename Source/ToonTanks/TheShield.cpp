// Fill out your copyright notice in the Description page of Project Settings.


#include "TheShield.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

// Sets default values
ATheShield::ATheShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	RootComponent = ShieldMesh;
}

// Called when the game starts or when spawned
void ATheShield::BeginPlay()
{
	Super::BeginPlay();
	
	//Delegate that broadcasts the designated function whenever it is hit by the projectile
	ShieldMesh->OnComponentHit.AddDynamic(this, &ATheShield::SuccessfulBlock);

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void ATheShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//after a successful block, adddynamic calls successful block and ammo is added through the add ammo method on tank
void ATheShield::SuccessfulBlock(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("hit"));
	Tank->AddAmmo();
	Tank->UnBlock();
}

