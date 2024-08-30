// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriStrikeGameMode.h"
#include "TetriStrikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATetriStrikeGameMode::ATetriStrikeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// set meshes and materials
	MeshArray.Init(nullptr, 5);
	MaterialArray.Init(nullptr, 5);

	LoadMeshIntoArray(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/IMino.IMino'"), 0);
	LoadMeshIntoArray(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/LMino.LMino'"), 1);
	LoadMeshIntoArray(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/OMino.OMino'"), 2);
	LoadMeshIntoArray(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/TMino.TMino'"), 3);
	LoadMeshIntoArray(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/ZMino.ZMino'"), 4);

	LoadMaterialIntoArray(TEXT("/Script/Engine.Material'/Game/Materials/BackgroundMaterial_temp.BackgroundMaterial_temp'"), 0);
	LoadMaterialIntoArray(TEXT("/Script/Engine.Material'/Game/Materials/BaseMaterial1.BaseMaterial1'"), 1);
	LoadMaterialIntoArray(TEXT("/Script/Engine.Material'/Game/Materials/CameraLitVertexColor_temp.CameraLitVertexColor_temp'"), 2);
	LoadMaterialIntoArray(TEXT("/Script/Engine.Material'/Game/Materials/CameraLitVertexColor_temp2.CameraLitVertexColor_temp2'"), 3);
	LoadMaterialIntoArray(TEXT("/Script/Engine.Material'/Game/Materials/M_Procedural_Sky_MASTER03.M_Procedural_Sky_MASTER03'"), 4);

	
}

void ATetriStrikeGameMode::LoadMeshIntoArray(const TCHAR* MeshPath, const int32 Index)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(MeshPath);
	if (MeshFinder.Succeeded())
	{
		MeshArray[Index] = MeshFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load mesh at %s"), MeshPath);
	}
}

void ATetriStrikeGameMode::LoadMaterialIntoArray(const TCHAR* MaterialPath, const int32 Index)
{
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(MaterialPath);
	if (MaterialFinder.Succeeded())
	{
		MaterialArray[Index] = MaterialFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material at %s"), MaterialPath);
	}
}
