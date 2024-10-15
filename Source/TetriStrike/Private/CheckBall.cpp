// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckBall.h"

#include "ClearZone.h"
#include "Components/SphereComponent.h"
#include "TetriStrike/TetriStrikeGameMode.h"

TArray<int32> ACheckBall::DensityArray;
int32 ACheckBall::Threshold;
ATetriStrikeGameMode* ACheckBall::GameMode;

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

	DensityArray.Init(0, 10);
}

// Called when the game starts or when spawned
void ACheckBall::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());

	
	SphComp->OnComponentBeginOverlap.AddDynamic(this,&ACheckBall::OnBeginOverlap);
	SphComp->OnComponentEndOverlap.AddDynamic(this, &ACheckBall::OnEndOverlap);
}

// Called every frame
void ACheckBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckBall::ModifyDensity(int32 Index, bool bIsOverlap)
{
	if (bIsOverlap)
	{
		DensityArray[Index]++;
		if (DensityArray[Index] > Threshold)
		{
			AClearZone::ClearArray[Index]->SliceAndDestroy();
			// GameMode->ClearArray[Index]->SliceAndDestroy();
		}
		
	}
	else
	{
		DensityArray[Index]--;
	}
}

void ACheckBall::DebugDensity()
{
	if (GEngine)
	{
		for (int Index = 0; Index < DensityArray.Num(); ++Index)
		{
			GEngine->AddOnScreenDebugMessage(Index, 1.0f, FColor::Blue,
				FString::Printf(TEXT("%d: %d"), Index, DensityArray[Index]));
		}
	}
}

void ACheckBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ModifyDensity(LayerIndex, true);
}

void ACheckBall::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ModifyDensity(LayerIndex, false);
}

