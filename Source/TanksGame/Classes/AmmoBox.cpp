// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"

#include "EnemyTank.h"
#include "UnitBase.h"


// Sets default values
AAmmoBox::AAmmoBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUnitBase* Tank = Cast<AUnitBase>(OtherActor))
	{
		ACannon* Cannon = Tank->GetActiveCannon();
		if (Cannon && Cannon->GetClass() == CannonClass)
		{
			Cannon->AddAmmo(AmmoAmount);
		}
		else
		{
			Tank->SetupCannon(CannonClass);
		}

		if(!bIsInfiniteAmmo)
			Destroy();
	}
}