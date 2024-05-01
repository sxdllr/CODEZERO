
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

class ABaseCharacter;
class AProjectile;
class USkeletalMesh;
class USphereComponent;
class UArrowComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class CODEZERO_API ABaseGun : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* GunCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* FireMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	
public:	
	ABaseGun();

	void AttachWeapon(ABaseCharacter* OtherCharacter);

	//void SetStats(ETriggerEvent OtherTrigger);

	USkeletalMeshComponent* GetGunMesh() const { return GunMesh; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnFire() {};

	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	void ResetFireRate();

	bool bCanFire = true;
	
private:
	//ETriggerEvent FireTrigger;
};
