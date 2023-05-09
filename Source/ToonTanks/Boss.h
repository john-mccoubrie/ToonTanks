// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ABoss : public ABasePawn
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();


protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	class ATank* Tank;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float FireRange = 2000.f;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 1.f;
	void CheckFireCondition();

	bool InFireRange();

	void Move(float Value);


};
