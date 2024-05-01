
#include "Inventory/ComponentPicker.h"
#include "Character/BaseCharacter.h"
#include "Weapon/BaseGun.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h"

AComponentPicker::AComponentPicker()
    : PickUpCollision(nullptr)
    , bIsOverlap(false)
    , PickUpRadius(64.f)
{
    PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    PickUpCollision->SetSphereRadius(PickUpRadius);
    PickUpCollision->SetCollisionProfileName("PickUpRadius");
    PickUpCollision->SetHiddenInGame(false);
    RootComponent = PickUpCollision;

    PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AComponentPicker::BeginOverlap);
    PickUpCollision->OnComponentEndOverlap.AddDynamic(this, &AComponentPicker::EndOverlap);
}

void AComponentPicker::BeginPlay()
{
    Super::BeginPlay();
}

void AComponentPicker::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
    {
        OnPickUp.Broadcast(Character);
        bIsOverlap = true;
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, "BeginOverlap Succeed");
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "BeginOverlap Failed");
    }
    // add outline when entering the radius of weapon raising
}

void AComponentPicker::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bIsOverlap = false;
    // break outline when leaving the radius of weapon raising
}

ABaseGun* AComponentPicker::FindGunToPickUp()
{
    TArray<AActor*> OverlappingActors;
    PickUpCollision->GetOverlappingActors(OverlappingActors, ABaseGun::StaticClass());

    for (AActor* Actor : OverlappingActors)
    {
        if (ABaseGun* Gun = Cast<ABaseGun>(Actor))
        {
            return Gun;
        }
    }
    
    return nullptr;
}

void AComponentPicker::PickUpGunRadius(ABaseCharacter* Character)
{
    if (bIsOverlap)
    {
        OnPickUp.Broadcast(Character);
        
        if (ABaseGun* FoundGun = FindGunToPickUp())
        {
            FoundGun->AttachWeapon(Character); // later send to Inventory
        }
    }
}