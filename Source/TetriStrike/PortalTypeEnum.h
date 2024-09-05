#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EPortalType : uint8
{
	Floor,
	Front,
	Right,
	Left,
	Back,
	Not_Valid
};