// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckBall.h"

#include "Components/SphereComponent.h"
#include "TetriStrike/TetriStrikeGameMode.h"


// Sets default values
ACheckBall::ACheckBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphComp"));
	SphComp->SetSphereRadius(1.0f);
	// tick false 가 정상적으로 작동하는지 확인해야함
	SphComp->PrimaryComponentTick.bCanEverTick = false;
	RootComponent = SphComp;

	SphComp->SetCollisionProfileName(TEXT("NoCollision"));
	SphComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
}

// Called when the game starts or when spawned
void ACheckBall::BeginPlay()
{
	Super::BeginPlay();
	
	SphComp->OnComponentBeginOverlap.AddDynamic(this,&ACheckBall::OnBeginOverlap);
	SphComp->OnComponentEndOverlap.AddDynamic(this, &ACheckBall::OnEndOverlap);
}

// Called every frame
void ACheckBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* GameMode = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->ModifyDensity(LayerIndex, true);
}

void ACheckBall::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto GameMode = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->ModifyDensity(LayerIndex, false);
}
