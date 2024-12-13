// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalTypeEnum.h"

#include "PortalSpawner.generated.h"

UCLASS()
class TETRISTRIKE_API APortalSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortalSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool bShouldCreatePortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPortalType PortalType = EPortalType::Not_Valid;
};
