// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinoSpawner.generated.h"

UCLASS()
class TETRISTRIKE_API AMinoSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinoSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnAndMoveMino() const;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMino> MinoToSpawn;

	// change this value
	UPROPERTY(EditAnywhere)
	float InitSpeed = 300.0f;

	static FRotator GetRandom90DegreeRotation();
};
