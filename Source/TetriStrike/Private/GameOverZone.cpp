// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverZone.h"

#include "TetriStrike/TetriStrikeGameMode.h"


// Sets default values
AGameOverZone::AGameOverZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComp"));
	RootComponent = PlaneComp;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (PlaneMesh.Succeeded())
	{
		PlaneComp->SetStaticMesh(PlaneMesh.Object);
	}
	// collision channel "GameOverZone"
	PlaneComp->SetCollisionObjectType(ECC_GameTraceChannel3);
	PlaneComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Mino overlap
	PlaneComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
}

// Called when the game starts or when spawned
void AGameOverZone::BeginPlay()
{
	Super::BeginPlay();

	PlaneComp->OnComponentBeginOverlap.AddDynamic(this, &AGameOverZone::OnPlaneCompBeginOverlap);
}

void AGameOverZone::OnPlaneCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* GameMode = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameOver"));
		GameMode->ShowGameOver();
	}
}

// Called every frame
void AGameOverZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

