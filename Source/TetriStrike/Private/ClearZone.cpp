// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearZone.h"

#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Chaos/AABBTree.h"
#include "Components/BoxComponent.h"


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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
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
}

// Called when the game starts or when spawned
void AClearZone::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &AClearZone::SliceAndDestroy, 10.0f, true);
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

	UE_LOG(LogTemp, Warning, TEXT("Num %d"), OverlapArray.Num());
	for (auto* Component: OverlapArray)
	{
		if (Component)
		{
			UProceduralMeshComponent* ProMeshComp = Cast<UProceduralMeshComponent>(Component);
			if (ProMeshComp)
			{
				FVector SlicePos = CeilingPlane->GetComponentLocation();
				UKismetProceduralMeshLibrary::SliceProceduralMesh(ProMeshComp, SlicePos,
					FVector(0, 0, 1), false,
					ProMeshComp, EProcMeshSliceCapOption::UseLastSectionForCap, nullptr);
			}
		}
	}
}

void AClearZone::SliceUp() const
{
	TArray<UPrimitiveComponent*> OverlapArray;
	FloorPlane->GetOverlappingComponents(OverlapArray);

	UE_LOG(LogTemp, Warning, TEXT("Num %d"), OverlapArray.Num());
	for (auto* Component: OverlapArray)
	{
		if (Component)
		{
			auto* ProMeshComp = Cast<UProceduralMeshComponent>(Component);
			if (ProMeshComp)
			{
				FVector SlicePos = FloorPlane->GetComponentLocation();
				UKismetProceduralMeshLibrary::SliceProceduralMesh(ProMeshComp, SlicePos,
					FVector(0, 0, -1), false,
					ProMeshComp, EProcMeshSliceCapOption::UseLastSectionForCap, nullptr);
			}
		}
	}
}

void AClearZone::DestroyCentor() const
{
	TArray<UPrimitiveComponent*> OverlapArray;
	BoxComp->GetOverlappingComponents(OverlapArray);

	UE_LOG(LogTemp, Warning, TEXT("Num %d"), OverlapArray.Num());
	for (auto* Component: OverlapArray)
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
	SliceUp();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &AClearZone::SliceDown, 0.05f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle3, this, &AClearZone::DestroyCentor, 0.05f, false);
}
