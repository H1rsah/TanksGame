#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructures.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8 {
	FireProjectile = 0 UMETA(DisplayName = "Projectile"),
	FireTrace = 1 UMETA(DisplayName = "Trace")
};

UENUM(BlueprintType)
enum class EUnitMobility : uint8 {
	Static = 0 UMETA(DisplayName = "Static"),
	Mobile = 1 UMETA(DisplayName = "Mobile")
};

USTRUCT(BlueprintType)
struct FDamageTypes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* DamageMaker;
};
