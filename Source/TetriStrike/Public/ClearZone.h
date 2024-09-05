// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClearZone.generated.h"

UCLASS()
class TETRISTRIKE_API AClearZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClearZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CeilingPlane;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FloorPlane;
	
	UPROPERTY()
	FTimerHandle TimerHandle1;
	UPROPERTY()
	FTimerHandle TimerHandle2;
	UPROPERTY()
	FTimerHandle TimerHandle3;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
	class USoundBase* Sound;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SliceDown() const;
	UFUNCTION()
	void SliceUp() const;
	UFUNCTION()
	void DestroyCentor() const;

	UFUNCTION()
	void SliceAndDestroy();

	UFUNCTION()
	void SpawnEffect();

	UFUNCTION()
	void SpawnSound();
};
