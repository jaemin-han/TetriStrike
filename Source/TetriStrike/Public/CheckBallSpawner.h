// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckBallSpawner.generated.h"

UCLASS()
class TETRISTRIKE_API ACheckBallSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckBallSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACheckBall> Target;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnCheckBalltoWorld(FVector StartPos, float XLength, float YLength, float ZLength,
		float CubeRadius, int32 NumLayer);

private:
	ACheckBall* SpawnOneBall(FVector Pos);
	FVector GetCubeCenterPos(int32 XNum, int32 YNum, int32 ZNum, float CubeRadius);
	
};
