#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "S_DamageTypes.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8 {
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use trace")
};


USTRUCT()
struct FS_DamageTypes
{
	GENERATED_BODY()

};
