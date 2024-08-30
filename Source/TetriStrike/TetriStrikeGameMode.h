// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetriStrikeGameMode.generated.h"

UCLASS(minimalapi)
class ATetriStrikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATetriStrikeGameMode();

	UPROPERTY(EditAnywhere)
	TArray<class UStaticMesh*> MeshArray;
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> MaterialArray;

private:
	void LoadMeshIntoArray(const TCHAR* MeshPath, const int32 Index);
	void LoadMaterialIntoArray(const TCHAR* MaterialPath, const int32 Index);
};



