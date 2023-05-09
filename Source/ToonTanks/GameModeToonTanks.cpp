// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeToonTanks.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "Boss.h"
#include "ToonTanksPlayerController.h"

//whenever an actor dies call this method to check which actor, game over, and actor counts
void AGameModeToonTanks::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
		
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TargetTowers--;
		if (TargetTowers == 0 && TargetBoss == 0)
		{
			TowersDead = true;
		}
	}
	else if (ABoss* DestroyedBoss = Cast<ABoss>(DeadActor))
	{
		DestroyedBoss->HandleDestruction();
		TargetBoss--;
		BossesDead = true;
	}
	if (TowersDead && BossesDead)
	{
		GameOver(true);
	}
}

void AGameModeToonTanks::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();

}

void AGameModeToonTanks::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate, 
			StartDelay, 
			false);
	}
}

int32 AGameModeToonTanks::GetTargetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
	return Towers.Num();
}

int32 AGameModeToonTanks::GetBossCount()
{
	TArray<AActor*> Boss;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Boss);
	return Boss.Num();
}