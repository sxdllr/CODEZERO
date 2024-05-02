
#include "Weapon/BaseGun.h"
#include "Character/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"

ABaseGun::ABaseGun()
	: GunMesh(nullptr)
	, GunCollision(nullptr)
	, FireMappingContext(nullptr)
	, FireAction(nullptr)
	, Character(nullptr)
	, ProjectileClass()
	, bCanFire(true)
	, FireTrigger(ETriggerEvent::None)
{
	PrimaryActorTick.bCanEverTick = false;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = GunMesh;

	GunCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	GunCollision->SetSphereRadius(10.f);
	GunCollision->SetupAttachment(GunMesh);
}

void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGun::AttachWeapon(ABaseCharacter* OtherCharacter)
{
	Character = OtherCharacter;

	if (!Character || Character->GetComponentByClass(ABaseGun::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "FailedToAttach");
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh3P(), AttachmentRules);

	SetOwner(Character);
	Character->SetHasGun(true);

	if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, FireTrigger, this, &ABaseGun::OnFire);
		}
	}
}

void ABaseGun::SetStats(ETriggerEvent OtherTrigger)
{
	FireTrigger = OtherTrigger;
}

void ABaseGun::ResetFireRate()
{
	bCanFire = true;
}

