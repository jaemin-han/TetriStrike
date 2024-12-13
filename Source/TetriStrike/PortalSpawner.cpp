// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalSpawner.h"
#include "TetriStrikeGameMode.h"
// Sets default values
APortalSpawner::APortalSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APortalSpawner::BeginPlay()
{
	Super::BeginPlay();
	ATetriStrikeGameMode* gm = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());

	if (gm->PortalType != EPortalType::Not_Valid && gm->PortalCount < 2)
	{
		PortalType = gm->PortalType;
		gm->PortalCount++;
		gm->PortalType = EPortalType::Not_Valid;
	}
	else
	{
		Destroy();
	}
}
