// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	public:
	ATank();
	void AddAmmo();

	UFUNCTION(BlueprintPure)
	bool GetDodgeUI();

	UFUNCTION(BlueprintPure)
	float GetAmmoUI();

	UPROPERTY(EditAnywhere, Category = "Combat")
		bool IsDodgeActive = true;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DodgeTimerHandle;
	FTimerHandle DodgeRespawnHandle;

	private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 800.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 120.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxAmmo = 3;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float CurrentAmmo = 5;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Reload = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DodgeSpawnDelay = 0.15;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DodgeActiveDelay = 2;

	

	void Move(float Value);
	void Turn(float Value);
	void CanShoot();
	void StartDodge();
	void EndDodge();
	void SetDodge();

	APlayerController* TankPlayerController;
};
