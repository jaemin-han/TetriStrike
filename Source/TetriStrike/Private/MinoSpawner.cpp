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
	
	// MinoToSpawn가 nullptr인지 확인합니다. 스폰할 미노의 클래스가 설정되어 있는 경우에만 작업을 계속합니다.
	if (MinoToSpawn != nullptr)
	{
		// SpawnLocation: 현재 AMinoSpawner의 위치에서 미노를 스폰합니다.
		const FVector SpawnLocation = GetActorLocation();
		// SpawnRotation: 90도 단위로 랜덤하게 회전된 방향을 설정합니다.
		const FRotator SpawnRotation = GetRandom90DegreeRotation();
		/*SpawnActor를 사용하여 AMino 클래스를 스폰합니다.
		스폰할 클래스는 MinoToSpawn, 위치는 SpawnLocation, 회전은 SpawnRotation으로 설정됩니다.
		성공적으로 스폰되면 SpawnedMino 변수에 스폰된 객체가 저장됩니다.*/
		AMino* SpawnedMino = GetWorld()->SpawnActor<AMino>(MinoToSpawn, SpawnLocation, SpawnRotation);

		 
		if (SpawnedMino)
		{
			//스폰된 미노의 루트 컴포넌트를 UPrimitiveComponent로 캐스팅합니다. UPrimitiveComponent는
			// 물리적 속성(예: 충돌, 물리 시뮬레이션)을 가진 컴포넌트입니다.
			UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(SpawnedMino->GetRootComponent());
			//Velocity = FVector(0, 0, -InitSpeed): Z축으로 -InitSpeed의 값을 갖는 벡터를 설정하여 미노가 아래로 떨어지게 합니다.
			if (RootPrimitiveComponent && RootPrimitiveComponent->IsSimulatingPhysics())
			{
				const FVector Velocity = FVector(0, 0, -InitSpeed);
				//SetPhysicsLinearVelocity는 객체에 물리적 속도를 적용하는 함수입니다.
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

