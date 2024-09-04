// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckBallSpawner.h"

#include "CheckBall.h"
#include "ClearZone.h"
#include "TetriStrike/TetriStrikeGameMode.h"


// Sets default values
ACheckBallSpawner::ACheckBallSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACheckBallSpawner::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());
	
	// left bottom position
	SetActorLocation(FVector(-500.0f, -500.0f, 0));

	SpawnCheckBall(GetActorLocation(), 1000.0f, 1000.0f, 1000.0f,
		25.0f, 10, GameMode->ThresholdRatio);
	SpawnClearZone();
}

// Called every frame
void ACheckBallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckBallSpawner::SpawnCheckBall(FVector StartPos, float XLength, float YLength, float ZLength,
	float CubeRadius, int32 NumLayer, float ThresholdRatio)
{
	ThresholdRatio = FMath::Clamp(ThresholdRatio,0.0f, 1.0f);

	int32 XNum = static_cast<int32>(XLength / (2 * CubeRadius));
	int32 YNum = static_cast<int32>(YLength / (2 * CubeRadius));
	int32 ZNum = static_cast<int32>(ZLength / (2 * CubeRadius));

	GameMode->Threshold = static_cast<int32>(XNum * YNum * ZNum / NumLayer * ThresholdRatio);

	int32 LayerHeight = FMath::Max(1, ZNum / NumLayer);
	int32 CurrentLayerIndex = -1;
	
	for (int k = 1; k <= ZNum; ++k)
	{
		if ((k - 1) % LayerHeight == 0)
		{
			CurrentLayerIndex++;
		}
		
		for (int i = 1; i <= XNum; ++i)
		{
			for (int j = 1; j <= YNum; ++j)
			{
				ACheckBall* SpawnedBall = SpawnOneBall(StartPos + GetCubeCenterPos(i, j, k, CubeRadius));
				SpawnedBall->LayerIndex = CurrentLayerIndex;
			}
		}
	}
}

void ACheckBallSpawner::SpawnClearZone()
{
	float ZPos = 50.0f;

	for (int32 Index = 0; Index < 10; ++Index)
	{
		if (IsValid(ClearZone))
		{
			auto* SpawnedClearZone = GetWorld()->SpawnActor<AClearZone>(ClearZone, FVector(0, 0, ZPos + 100 * Index), FRotator());
			GameMode->ClearArray.Add(SpawnedClearZone);
		}
	}
}

ACheckBall* ACheckBallSpawner::SpawnOneBall(FVector Pos)
{
	if (IsValid(CheckBall))
	{
		ACheckBall* SpawnedBall = GetWorld()->SpawnActor<ACheckBall>(CheckBall, Pos, FRotator(0, 0, 0));
		return SpawnedBall;
	}
	UE_LOG(LogTemp, Error, TEXT("Cannot Spawn in ACheckBallSpawner::SpawnOneBall"));
	return nullptr;
}

FVector ACheckBallSpawner::GetCubeCenterPos(int32 XNum, int32 YNum, int32 ZNum, float CubeRadius)
{
	float XLen = (2 * XNum - 1) * CubeRadius;
	float YLen = (2 * YNum - 1) * CubeRadius;
	float ZLen = (2 * ZNum - 1) * CubeRadius;

	return {XLen, YLen, ZLen};
}


