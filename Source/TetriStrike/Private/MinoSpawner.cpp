// Fill out your copyright notice in the Description page of Project Settings.


#include "MinoSpawner.h"

#include "Mino.h"

// Sets default values
AMinoSpawner::AMinoSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinoSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMinoSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinoSpawner::SpawnAndMoveMino() const
{
	if (MinoToSpawn != nullptr)
	{
		const FVector SpawnLocation = GetActorLocation();
		const FRotator SpawnRotation = GetRandom90DegreeRotation();

		AMino* SpawnedMino = GetWorld()->SpawnActor<AMino>(MinoToSpawn, SpawnLocation, SpawnRotation);

		if (SpawnedMino)
		{
			UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(SpawnedMino->GetRootComponent());
			if (RootPrimitiveComponent && RootPrimitiveComponent->IsSimulatingPhysics())
			{
				const FVector Velocity = FVector(0, 0, -InitSpeed);
				RootPrimitiveComponent->SetPhysicsLinearVelocity(Velocity);
			}
		}
	}
}

FRotator AMinoSpawner::GetRandom90DegreeRotation()
{
	const int32 RandomPitch = FMath::RandRange(0, 3) * 90;
	const int32 RandomYaw = FMath::RandRange(0, 3) * 90;
	const int32 RandomRoll = FMath::RandRange(0, 3) * 90;
	return FRotator(RandomPitch, RandomYaw, RandomRoll);
}

