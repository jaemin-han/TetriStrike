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

private:
	void LoadMeshIntoArray(const TCHAR* MeshPath, const int32 Index);
	void LoadMaterialIntoArray(const TCHAR* MaterialPath, const int32 Index);

public:
	//Portal transform
	UPROPERTY(BlueprintReadOnly)
	FVector PortalLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator PortalRotation;
	
	//Check to see if value is in
	UPROPERTY(BlueprintReadOnly)
	bool bTransformCheck = false;

	//Check if projectile is hit
	UPROPERTY(BlueprintReadOnly)
	bool bTransformChanged = false;
};