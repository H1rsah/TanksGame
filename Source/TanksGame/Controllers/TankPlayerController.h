#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TanksGame/Interfaaces/Targetable.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

UCLASS()
class TANKSGAME_API ATankPlayerController : public APlayerController, public ITargetable
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ATankPawn* TankPawn;
	FVector MousePosition;

public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	virtual FVector GetShootTarget() const override { return MousePosition; };

	UPROPERTY()
	int32 PlayerScore;

private:
	virtual void BeginPlay() override;
	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void Fire();
	void FireSpecial();
	void SwitchCannon();
};
