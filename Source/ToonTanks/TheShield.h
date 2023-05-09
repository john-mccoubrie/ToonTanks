
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "TheShield.generated.h"

UCLASS()
class TOONTANKS_API ATheShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATheShield();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ShieldMesh;

	class ATank* Tank;

	UFUNCTION()
	void SuccessfulBlock(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};