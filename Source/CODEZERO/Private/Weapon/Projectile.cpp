
#include "Weapon/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"

AProjectile::AProjectile()
	: CollisionComp(nullptr)
	, MeshComp(nullptr)
	, ProjectileMovement(nullptr)
	, ExplosionFX(nullptr)
	, BulletExpiry(0.f)
{
 	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(CollisionComp);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.5f;
	
	InitialLifeSpan = 3.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AProjectile::Explode, 3.0f, false);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawTraceLine(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        float RandomIntensity = FMath::RandRange(200.0f, 500.0f);
        OtherComp->AddImpulseAtLocation(GetVelocity() * RandomIntensity, GetActorLocation());

    	/*
    	APawn* HitPawn = Cast<APawn>(OtherActor);
    	if (HitPawn)
    	{
    		const float DamageAmount = 50.0f;
    		FDamageEvent DamageEvent;
    		OtherActor->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);
    	}
    	*/ //Add Logic to damage enemy class
    	
    	Destroy();
    }
}

void AProjectile::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));
	Destroy();
}

void AProjectile::DrawTraceLine(float DeltaTime)
{
	FVector StartLocation = this->GetActorLocation();
	FVector EndLocation = (ProjectileMovement->Velocity * DeltaTime) + StartLocation;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Destructible, CollisionParams))
	{
		if(HitResult.GetActor())
		{
			DrawDebugSolidBox(GetWorld(), HitResult.ImpactPoint, FVector(5.f), FColor::White, false, InitialLifeSpan);
			UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(HitResult.GetActor()->GetRootComponent());
			if (RootPrimitive && RootPrimitive->IsSimulatingPhysics())
			{
				float RandomIntensity = FMath::RandRange(5.0f, 25.0f);
				FVector Impulse = GetVelocity() * RandomIntensity;
				RootPrimitive->AddImpulse(Impulse);
			}
			Explode();
		}
	}
	else
	{
		BulletExpiry += DeltaTime;
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(0.f, -BulletExpiry * 80.f, 100.f), false, InitialLifeSpan);
		SetActorLocation(EndLocation);
	}

	if(BulletExpiry > InitialLifeSpan)
	{
		Destroy();
	}
}