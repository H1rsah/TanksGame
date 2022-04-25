// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"



AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.05f;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	Mesh->SetVisibility(true);
	RootComponent = Mesh;

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(Mesh);

}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	if (bEnableRadialDamage)
	{
		FVector StartPos = GetActorLocation();
		FVector EndPos = StartPos + FVector(0.1f);

		FCollisionShape Shape = FCollisionShape::MakeSphere(ExplosionRange);
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = true;
		Params.TraceTag = "Explode Trace";
		TArray<FHitResult> AttackHit;

		FQuat Rotation = FQuat::Identity;

		// GetWorld()->DebugDrawTraceTag = "Explode Trace";
		bool bSweepResult = GetWorld()->SweepMultiByChannel(AttackHit, StartPos, EndPos, Rotation, ECC_Visibility, Shape, Params);

		if (bSweepResult)
		{
			for (FHitResult SweepHitResult : AttackHit)
			{
				AActor* HitActor = SweepHitResult.GetActor();
				if (!HitActor)
					continue;

				FVector ForceVector = HitActor->GetActorLocation() - GetActorLocation();
				ApplyDamage(HitActor, Cast<UPrimitiveComponent>(HitActor->GetRootComponent()), SweepHitResult, ForceVector * ExplosionImpulse);
			}
		}
	}
	else
	{
		ApplyDamage(OtherActor, OtherComp, SweepResult, Mass * MoveSpeed * GetActorForwardVector());
	}
	
	AudioEffect->Play();
	Stop();
}

void AProjectileBase::ApplyDamage(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit, const FVector& Impulse)
{
	if (OtherActor == GetInstigator())
	{
		// Destroy();
		return;
	}
    
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
	}
    	
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
    	
	IDamageable* Damageable = Cast<IDamageable>(OtherActor);
	if (Damageable && GetOwner() != OtherActor)
	{
		FDamageTypes DamageType;
		DamageType.DamageValue = Damage;
		DamageType.Instigator = GetInstigator();
		DamageType.DamageMaker = this;
		Damageable->TakeDamage(DamageType);
	}
}


void AProjectileBase::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	StartPosition = GetActorLocation();
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectileBase::Move(float DeltaTime)
{
	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Stop();
	}
}

void AProjectileBase::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UActorPool* Pool = GetWorld()->GetSubsystem<UActorPool>();
	if (Pool->IsActorInPool(this))
	{
		Pool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}


