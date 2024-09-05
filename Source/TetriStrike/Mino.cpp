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
	// ProComp는 미노의 UPrimitiveComponent이며, 이 코드에서 충돌한 미노에 중력을 활성화합니다.
	// 중력이 활성화되면 미노는 중력의 영향을 받아 아래로 떨어지게 됩니다.
	ProComp->SetEnableGravity(true);


	/* 충돌한 액터(OtherActor)가 "Floor" 또는 "Mino" 태그를 가지고 있는지 확인합니다.
	"Floor" 태그는 바닥을 의미하며, "Mino" 태그는 다른 미노 블록을 의미합니다.
	즉, 미노가 바닥이나 다른 미노와 충돌했을 때만 아래 코드를 실행합니다.  */
	if (OtherActor->ActorHasTag(TEXT("Floor")) || OtherActor->ActorHasTag(TEXT("Mino")))
	{
		/*bCanSpawn은 새로운 미노를 스폰할 수 있는지 여부를 결정하는 불리언 변수입니다.
		이 변수가 true이면 새로운 미노를 스폰할 수 있습니다.
		새로운 미노를 스폰한 후, bCanSpawn을 false로 설정하여 다시 스폰되지 않도록 합니다.
		이로 인해 한 번의 충돌에서 한 개의 미노만 스폰됩니다. */
		if (!bCanSpawn)	return;
		bCanSpawn = false;

		ProComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

		/* 현재 월드의 게임 모드를 가져오고, 이를 ATetriStrikeGameMode로 캐스팅합니다.
		ATetriStrikeGameMode의 Spawner를 통해 새로운 미노를 스폰하고 이동시키는 SpawnAndMoveMino 함수를 호출합니다.
		즉, 충돌이 발생하면 새로운 미노를 스폰하고, 이전 미노는 위치에 고정됩니다. */
		auto* GameMode = GetWorld()->GetAuthGameMode();
		auto* TGameMode = Cast<ATetriStrikeGameMode>(GameMode);

		TGameMode->Spawner->SpawnAndMoveMino();
		
		/*충돌된 미노의 물리적 충돌 알림을 비활성화합니다.
		이로 인해 더 이상 충돌 이벤트가 발생하지 않으며, 충돌된 미노가 다른 미노와 충돌해도 추가적인 충돌 이벤트가 발생하지 않습니다.*/
		ProComp->SetNotifyRigidBodyCollision(false);
	}
}

