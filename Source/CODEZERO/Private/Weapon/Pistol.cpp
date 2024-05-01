
#include "Weapon/Pistol.h"
#include "Weapon/Projectile.h"
#include "Character/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Controller.h"

APistol::APistol()
{
	SetStats(ETriggerEvent::Triggered);
}

void APistol::OnFire()
{
	if (!Character || !Character->GetController())
	{
		return;
	}
	
	if (ProjectileClass && bCanFire)
	{
		bCanFire = false;
		
		const FVector SpawnLocation = GetGunMesh()->GetSocketLocation("bullet");
		FRotator SpawnRotator = FRotator::ZeroRotator;
		
		if (AActor* OwnerActor = GetOwner())
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(OwnerActor->GetInstigatorController()))
			{
				SpawnRotator = PlayerController->GetControlRotation();
			}
		}

		const float Spread = 1.0f;
		SpawnRotator.Yaw += FMath::RandRange(-Spread, Spread);
		SpawnRotator.Pitch += FMath::RandRange(-Spread, Spread);
		
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotator);
		FVector NewVelocity = GetActorForwardVector() * 10000.f;
		Projectile->Velocity = NewVelocity;

		FTimerHandle FireRate;
		GetWorld()->GetTimerManager().SetTimer(FireRate, this, &APistol::ResetFireRate, 0.1f, false);
	}
}

