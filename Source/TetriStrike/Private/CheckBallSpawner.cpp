// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckBallSpawner.h"

#include "CheckBall.h"


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

	// left bottom position
	SetActorLocation(FVector(-500.0f, -500.0f, 0));

	SpawnCheckBalltoWorld(GetActorLocation(), 1000.0f, 1000.0f, 1000.0f,
		25.0f, 10);
}

// Called every frame
void ACheckBallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckBallSpawner::SpawnCheckBalltoWorld(FVector StartPos, float XLength, float YLength, float ZLength,
	float CubeRadius, int32 NumLayer)
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnCheckBalltoWorld"));

	int32 XNum = static_cast<int32>(XLength / (2 * CubeRadius));
	int32 YNum = static_cast<int32>(YLength / (2 * CubeRadius));
	int32 ZNum = static_cast<int32>(ZLength / (2 * CubeRadius));

	for (int k = 1; k <= ZNum; ++k)
	{
		for (int i = 1; i <= XNum; ++i)
		{
			for (int j = 1; j <= YNum; ++j)
			{
				auto temp = GetCubeCenterPos(i, j, k, CubeRadius);
				UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), temp.X, temp.Y, temp.Z);
				ACheckBall* SpawnedBall = SpawnOneBall(StartPos + GetCubeCenterPos(i, j, k, CubeRadius));
				// Index 0 ~ 9, so -1 is needed
				SpawnedBall->LayerIndex = static_cast<int32>((k - 1) / 2);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), XNum, YNum, ZNum);
}

ACheckBall* ACheckBallSpawner::SpawnOneBall(FVector Pos)
{
	if (IsValid(Target))
	{
		ACheckBall* SpawnedBall = GetWorld()->SpawnActor<ACheckBall>(Target, Pos, FRotator(0, 0, 0));
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


