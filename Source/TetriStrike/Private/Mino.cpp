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

	const int32 Index1 = FMath::RandRange(0, GameMode->MeshArray.Num() - 1);
	const int32 Index2 = FMath::RandRange(0, GameMode->MeshArray.Num() - 1);

	UStaticMesh* MyMesh = GameMode->MeshArray[Index1];
	MeshComp->SetStaticMesh(MyMesh);

	UMaterial* MyMat = GameMode->MaterialArray[Index2];

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


/* AMino::OnMinoHit는 AMino 객체(테트리스의 미노 블록)가 다른 객체와 충돌했을 때 호출되는 함수입니다.
충돌한 대상이 특정 조건을 만족하면 새로운 미노를 스폰하고, 충돌된 미노에 대한 물리적 속성을 변경합니다.
HitComponent: 충돌한 미노의 UPrimitiveComponent.
OtherActor: 충돌한 다른 액터(예: 바닥이나 다른 미노).
OtherComp: 충돌한 다른 액터의 컴포넌트.
NormalImpulse: 충돌로 인한 힘 벡터.
Hit: 충돌의 세부 정보를 담고 있는 구조체.
*/
void AMino::OnMinoHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("OnMinoHit"));
	ProComp->SetEnableGravity(true);

	if (OtherActor->ActorHasTag(TEXT("Floor")) || OtherActor->ActorHasTag(TEXT("Mino")))
	{
		if (!bCanSpawn) return;
		bCanSpawn = false;
		// 추가적인 RigidBodyCollision 이 발생하지 않게 한다
		ProComp->SetNotifyRigidBodyCollision(false);
		// GameOverZone class 와 Overlap 충돌판정을 설정한다
		ProComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

		// 새로운 블록을 스폰한다
		auto* GameMode = GetWorld()->GetAuthGameMode();
		auto* TGameMode = Cast<ATetriStrikeGameMode>(GameMode);
		TGameMode->Spawner->SpawnAndMoveMino();
	}
}
