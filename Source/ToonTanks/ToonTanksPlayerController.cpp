// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Tank.h"
#include "HealthComponent.h"
#include "Tower.h"


void AToonTanksPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//sets up heads up display for the health bar and shield bar
	//Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
	bShowMouseCursor = bPlayerEnabled;
}




