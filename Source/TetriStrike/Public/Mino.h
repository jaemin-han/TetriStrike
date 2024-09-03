// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mino.generated.h"

UCLASS()
class TETRISTRIKE_API AMino : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMino();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitializeProComp();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* ProComp;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	bool bCanSpawn = true;

	UFUNCTION()
	void OnMinoHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

};
