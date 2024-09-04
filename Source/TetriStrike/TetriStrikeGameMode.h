// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetriStrikeGameMode.generated.h"

UCLASS(minimalapi)
class ATetriStrikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	ATetriStrikeGameMode();

	UPROPERTY(EditAnywhere)
	TArray<class UStaticMesh*> MeshArray;
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> MaterialArray;

	UPROPERTY(EditAnywhere)
	class AMinoSpawner* Spawner;

	UPROPERTY(EditAnywhere)
	TArray<int32> DensityArray;

	UPROPERTY(EditAnywhere)
	TArray<class AClearZone*> ClearArray;

	UFUNCTION(BlueprintCallable)
	void ModifyDensity(int32 Index, bool bIsOverlap);

	UPROPERTY(EditAnywhere)
	int32 Threshold;

	UPROPERTY(EditAnywhere)
	float ThresholdRatio = 0.25f;

	UFUNCTION()
	int32 GetScore() const {return Score;};

	UFUNCTION()
	void SetScore(const int32 NewScore){Score = NewScore;};
	
private:
	void LoadMeshIntoArray(const TCHAR* MeshPath, const int32 Index);
	void LoadMaterialIntoArray(const TCHAR* MaterialPath, const int32 Index);

	TArray<bool> bAlreadyClearedArray;
	void ResetClearState(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DebugDensityArray();

	UPROPERTY()
	int32 Score = 0;

	UPROPERTY(EditAnywhere)
	int32 ScoreIncrement = 100;
};



