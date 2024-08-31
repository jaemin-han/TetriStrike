// Fill out your copyright notice in the Description page of Project Settings.


#include "Mino.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KismetProceduralMeshLibrary.h"
#include "MinoSpawner.h"
#include "TetriStrike/TetriStrikeGameMode.h"

// Sets default values
AMino::AMino()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProComp"));
	ProComp->bUseComplexAsSimpleCollision = false;
	ProComp->SetSimulatePhysics(true);
	ProComp->SetEnableGravity(false);
	ProComp->SetNotifyRigidBodyCollision(true);
	RootComponent = ProComp;

	// collision setting
	ProComp->SetCollisionObjectType(ECC_GameTraceChannel2);
	ProComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetupAttachment(RootComponent);

	Tags.Add(FName("Mino"));
}

void AMino::InitializeProComp()
{
	ATetriStrikeGameMode* GameMode = Cast<ATetriStrikeGameMode>(UGameplayStatics::GetGameMode(this));

	const int32 Index = FMath::RandRange(0, GameMode->MeshArray.Num() - 1);
	
	UStaticMesh* MyMesh = GameMode->MeshArray[Index];
	MeshComp->SetStaticMesh(MyMesh);
	
	UMaterial* MyMat = GameMode->MaterialArray[Index];

	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(MeshComp, 0, ProComp, true);
	MeshComp->DestroyComponent();

	for (int32 MatIndex = 0; MatIndex < ProComp->GetNumMaterials(); ++MatIndex)
	{
		ProComp->SetMaterial(MatIndex, MyMat);
	}
}

// Called when the game starts or when spawned
void AMino::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeProComp();

	ProComp->OnComponentHit.AddDynamic(this, &AMino::OnMinoHit);
}

// Called every frame
void AMino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMino::OnMinoHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ProComp->SetEnableGravity(true);
	
	if (OtherActor->ActorHasTag(TEXT("Floor")) || OtherActor->ActorHasTag(TEXT("Mino")))
	{
		if (!bCanSpawn)	return;
		bCanSpawn = false;

		auto* GameMode = GetWorld()->GetAuthGameMode();
		auto* TGameMode = Cast<ATetriStrikeGameMode>(GameMode);

		TGameMode->Spawner->SpawnAndMoveMino();
		
		ProComp->SetNotifyRigidBodyCollision(false);
	}
}

