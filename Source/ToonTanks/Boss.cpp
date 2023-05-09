// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
		Move(DeltaTime);
	}
}

void ABoss::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABoss::CheckFireCondition, FireRate, true);
}

void ABoss::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	if (InFireRange() && Tank->bAlive)
	{
		Fire();
	}
}

bool ABoss::InFireRange()
{
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}



void ABoss::Move(float DeltaTime)
{
	/*
	
	
	if (CurrentLocation.Y <= 500)
	{
		TargetLocation.Y = 500;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, BossSpeed);
		SetActorLocation(NewLocation);
	}
	else if (CurrentLocation.Y >= 500)
	{
		TargetLocation.Y = -2000;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, BossSpeed);
		SetActorLocation(NewLocation);
		UE_LOG(LogTemp, Warning, TEXT("Hello"));
	}
	
	*/


	/*
	float BossSpeed = 350.f;
	FVector CurrentLocation = GetActorLocation();
	
	if (CurrentLocation.Y < -900)
	{
		CurrentLocation += GetActorForwardVector() * BossSpeed * DeltaTime;
		SetActorLocation(CurrentLocation);
	}

	*/

	
	//Move to right
		

	
	
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Tank->GetActorLocation();
	float BossSpeed = 350.f;
	//Move the boss towards the player (tank)
	//He is moving up towards the tank so he comes off of the ground -- make physics static, change position he moves towards
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, BossSpeed);
	SetActorLocation(NewLocation);
	

}
