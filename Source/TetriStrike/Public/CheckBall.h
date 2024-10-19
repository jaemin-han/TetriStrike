// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckBall.generated.h"

UCLASS()
class TETRISTRIKE_API ACheckBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	int32 LayerIndex;

	static TArray<int32> DensityArray;

	UFUNCTION(BlueprintCallable)
	static void DebugDensity();

	static int32 Threshold;

private:
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphComp;

	UFUNCTION()
	static void ModifyDensity(int32 Index, bool bIsOverlap);

	static class ATetriStrikeGameMode* GameMode;


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                    AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex,
	                    bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                  AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
};
