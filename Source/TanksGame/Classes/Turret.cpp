// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	RootComponent = Collision;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	BodyMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("Turret");
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>("Cannon Setup Point");
	CannonSetupPoint->SetupAttachment(TurretMesh);

	AttackSphere = CreateDefaultSubobject<USphereComponent>("Attack Sphere");
	AttackSphere->SetupAttachment(TurretMesh);
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnEndOverlap);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATurret::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &ATurret::OnDie);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurret::TakeDamage(const FDamageTypes& Damage)
{
	if(HealthComponent)
		HealthComponent->TakeDamage(Damage);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle MyTimer;
	GetWorld()->GetTimerManager().SetTimer(MyTimer, this, &ATurret::Tracking, TrackingRate, true);

	if (CannonClass)
	{
		auto Transform = CannonSetupPoint->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		Cannon->MaxAmmo=100;
		Cannon->AddAmmo(100);
	}
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	if(Cannon)
		Cannon->Destroy();
}

void ATurret::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                             UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult)
{
	OtherActors.Add(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(!Target.IsValid())
	{
		FindNextTarget();
	}
}

void ATurret::OnEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* OtherComponent, int I)
{
	OtherActors.Remove(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(Actor == Target)
	{
		FindNextTarget();
	}
}

void ATurret::OnHealthChanged(float X)
{
	GEngine->AddOnScreenDebugMessage(123, 2, FColor::Red, FString::Printf(TEXT("TURRET Health: %f; Damage: %f"), HealthComponent->CurrentHealth, X));
}

void ATurret::OnDie()
{
	Destroy();
}

void ATurret::FindNextTarget()
{
	float MinRange = 1000;
	Target = nullptr;
	for(auto Actor : OtherActors)
	{
		float Range = FVector::Distance(Actor->GetActorLocation(), TurretMesh->GetComponentLocation()); // GetActorLocation());
		if (Range < MinRange)
		{
			MinRange = Range;
			Target = Actor;
		}
		else
			Target = nullptr;
	}
}

void ATurret::Tracking()
{
	if (!Target.IsValid())
	{
		return;
	}
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), Target->GetActorLocation());
	TargetRotation.Roll = 90.f;
	TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(), TargetRotation, TrackingSpeed));

	FVector TrackingDirection = TurretMesh->GetForwardVector();
	FVector PlayerDirection = Target->GetActorLocation() - TurretMesh->GetComponentLocation(); // GetActorLocation();
	PlayerDirection.Normalize();
    float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TrackingDirection, PlayerDirection)));

	GEngine->AddOnScreenDebugMessage(0, 0, FColor::Yellow, FString::Printf(TEXT("Angle: %f;  AimSlack: %f"), Angle, AimSlack));
	
	if(Angle < AimSlack)
	{
		Fire();
	}
}

void ATurret::Fire()
{
	if(Cannon)
		Cannon->Fire();
}


