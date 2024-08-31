// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TetriStrike/TetriStrikeProjectile.h"
#include "TetriStrikeReverseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TETRISTRIKE_API ATetriStrikeReverseProjectile : public ATetriStrikeProjectile
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay();
};
