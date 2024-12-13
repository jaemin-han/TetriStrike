// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearZone.h"

#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

TArray<AClearZone*> AClearZone::ClearArray;

// Sets default values
AClearZone::AClearZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// component setting
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	CeilingPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingPlane"));
	CeilingPlane->SetupAttachment(RootComponent);

	FloorPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorPlane"));
	FloorPlane->SetupAttachment(RootComponent);

	// Applying Plane Mesh 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (PlaneMesh.Succeeded())
	{
		CeilingPlane->SetStaticMesh(PlaneMesh.Object);
		FloorPlane->SetStaticMesh(PlaneMesh.Object);
	}

	// Do Detail in BluePinrt
	// const FVector BoxExtent(500, 500, 50);
	// BoxComp->SetBoxExtent(BoxExtent);
	//
	// const FVector PlaneScale(BoxExtent.X / 50.0f, BoxExtent.Y / 50.0f, 1.0f);
	// CeilingPlane->SetRelativeScale3D(PlaneScale);
	// FloorPlane->SetRelativeScale3D(PlaneScale);
	//
	// CeilingPlane->SetRelativeLocation(FVector(0, 0, BoxExtent.Z));
	// FloorPlane->SetRelativeLocation(FVector(0, 0, -BoxExtent.Z));

	// collision setting
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	CeilingPlane->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CeilingPlane->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	FloorPlane->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FloorPlane->SetCollisionResponseToAllChannels(ECR_Ignore);
	FloorPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	// visible setting
	CeilingPlane->SetVisibility(false);
	FloorPlane->SetVisibility(false);

	// spawn clear zone
}

// Called when the game starts or when spawned
void AClearZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AClearZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClearZone::SliceDown() const
{
	TArray<UPrimitiveComponent*> OverlapArray;
	CeilingPlane->GetOverlappingComponents(OverlapArray);

	// if (OverlapArray.Num() == 0)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No overlapping components found"));
	// 	return;
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("overlapping components Num: %d"), OverlapArray.Num());
	//
	// }

	UE_LOG(LogTemp, Warning, TEXT("SliceDown Num %d"), OverlapArray.Num());
	for (auto* Component : OverlapArray)
	{
		// Component가 유효한지 확인
		if (!Component)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid component (null) in OverlapArray"));
			continue;
		}

		if (Component)
		{
			UProceduralMeshComponent* ProMeshComp = Cast<UProceduralMeshComponent>(Component);
			if (ProMeshComp)
			{
				FVector SlicePos = CeilingPlane->GetComponentLocation();
				UKismetProceduralMeshLibrary::SliceProceduralMesh(ProMeshComp, SlicePos,
				                                                  FVector(0, 0, 1), false,
				                                                  ProMeshComp,
				                                                  EProcMeshSliceCapOption::UseLastSectionForCap,
				                                                  nullptr);
			}
		}
	}
}

void AClearZone::SliceUp() const
{
	TArray<UPrimitiveComponent*> OverlapArray;
	FloorPlane->GetOverlappingComponents(OverlapArray);

	UE_LOG(LogTemp, Warning, TEXT("SliceUp Num %d"), OverlapArray.Num());
	for (auto* Component : OverlapArray)
	{
		if (Component)
		{
			auto* ProMeshComp = Cast<UProceduralMeshComponent>(Component);
			if (ProMeshComp)
			{
				FVector SlicePos = FloorPlane->GetComponentLocation();
				UKismetProceduralMeshLibrary::SliceProceduralMesh(ProMeshComp, SlicePos,
				                                                  FVector(0, 0, -1), false,
				                                                  ProMeshComp,
				                                                  EProcMeshSliceCapOption::UseLastSectionForCap,
				                                                  nullptr);
			}
		}
	}
}

void AClearZone::DestroyCentor() const
{
	TArray<UPrimitiveComponent*> OverlapArray;
	BoxComp->GetOverlappingComponents(OverlapArray);

	UE_LOG(LogTemp, Warning, TEXT("DestroyCentor Num %d"), OverlapArray.Num());
	for (auto* Component : OverlapArray)
	{
		if (Component)
		{
			auto* ProMeshComp = Cast<UProceduralMeshComponent>(Component);
			if (ProMeshComp)
			{
				ProMeshComp->DestroyComponent(false);
			}
		}
	}
}

void AClearZone::SliceAndDestroy()
{
	if (bIsDestroying)
	{
		// UE_LOG(LogTemp, Warning, TEXT("SliceAndDestroy is already in progress."));
		return;
	}
	bIsDestroying = true;


	SliceDown();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &AClearZone::SliceUp, 0.0625f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle3, this, &AClearZone::DestroyCentor, 0.0625f, false);
	SpawnEffect();
	SpawnSound();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, [this]()
	{
		bIsDestroying = false;
		UE_LOG(LogTemp, Warning, TEXT("SliceAndDestroy completed, ready for next execution."));
	}, 1.0f, false);
}


void AClearZone::SpawnEffect()
{
	if (Effect)
	{
		// ClearZone의 중심 위치
		FVector CenterLocation = GetActorLocation();

		// 그리드 설정
		const int32 GridSize = 5; // 5x5 그리드
		const float GridSpacing = 200.0f; // 각 이펙트 간의 간격

		// 이펙트를 5x5 그리드에 맞춰 스폰
		for (int32 X = 0; X < GridSize; ++X)
		{
			for (int32 Y = 0; Y < GridSize; ++Y)
			{
				// 각 위치의 좌표를 계산
				FVector SpawnLocation = CenterLocation;
				SpawnLocation.X += (X - GridSize / 2) * GridSpacing;
				SpawnLocation.Y += (Y - GridSize / 2) * GridSpacing;

				// Z축은 CenterLocation의 Z와 동일하게 유지
				SpawnLocation.Z = CenterLocation.Z;

				// 파티클 이펙트 스폰
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, SpawnLocation, FRotator::ZeroRotator);
			}
		}
	}
}

void AClearZone::SpawnSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}
