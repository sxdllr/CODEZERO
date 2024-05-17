
#include "Inventory/ComponentPicker.h"
#include "Character/BaseCharacter.h"
#include "Weapon/BaseGun.h"
#include "Interfaces/InteractionInterface.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

AComponentPicker::AComponentPicker()
    : PickUpCollision(nullptr)
    , FoundGun(nullptr)
    , OverlappedCharacter(nullptr)
    , bIsOverlap(false)
    , PickUpRadius(64.f)
{
    PrimaryActorTick.bCanEverTick = true;
    
    PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    PickUpCollision->SetSphereRadius(PickUpRadius);
    PickUpCollision->SetCollisionProfileName("PickUpRadius");
    PickUpCollision->SetHiddenInGame(false);
    RootComponent = PickUpCollision;

    PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AComponentPicker::BeginOverlap);
    PickUpCollision->OnComponentEndOverlap.AddDynamic(this, &AComponentPicker::EndOverlap);

    PickUpCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickUpCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PickUpCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AComponentPicker::BeginPlay()
{
    Super::BeginPlay();
}

void AComponentPicker::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsOverlap && OverlappedCharacter)
    {
        InteractionCheck(OverlappedCharacter);
    }
}

void AComponentPicker::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
    {
        OnPickUp.Broadcast(Character);
        OverlappedCharacter = Character;
        //BeingOverlapped(OtherActor); fix this
        bIsOverlap = true;
    }
}

void AComponentPicker::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bIsOverlap = false;
    OverlappedCharacter = nullptr;
}

void AComponentPicker::InteractionCheck(const ACharacter* OtherActor)
{
    InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector StartTrace = OtherActor->GetPawnViewLocation();
    FVector EndTrace = (StartTrace + OtherActor->GetViewRotation().Vector() * PickUpRadius * 3.f);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OtherActor);
    Params.AddIgnoredComponent(PickUpCollision);
    FHitResult HitTrace;

    DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::White, false, 3.f, 0 , 2.f);

    if (GetWorld()->LineTraceSingleByChannel(HitTrace, StartTrace, EndTrace, ECC_Visibility, Params))
    {
        if (HitTrace.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
        {
            InteractionData.CurrentInteractable = HitTrace.GetActor();
            FoundInteractable(HitTrace.GetActor());
            
            if (HitTrace.GetActor() != InteractionData.CurrentInteractable)
            {
                //InteractionData.CurrentInteractable = HitTrace.GetActor();
                //FoundInteractable(HitTrace.GetActor());
                //return;
            }
        }
    }
}

void AComponentPicker::BeingOverlapped(AActor* NewInteractable)
{
    InteractionData.CurrentInteractable = NewInteractable;
    
    if (NewInteractable->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
    {
        IInteractionInterface* InteractableInterface = Cast<IInteractionInterface>(NewInteractable);
        if (InteractableInterface)
        {
            InteractableInterface->OnEnterOverlap();
        }
    }
}

void AComponentPicker::FoundInteractable(AActor* NewInteractable)
{
    InteractionData.CurrentInteractable = NewInteractable;
    
    if (NewInteractable->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
    {
        IInteractionInterface* InteractableInterface = Cast<IInteractionInterface>(NewInteractable);
        if (InteractableInterface)
        {
            InteractableInterface->OnInteract();
        }
    }
}

ABaseGun* AComponentPicker::FindItemToPickUp() const
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

void AComponentPicker::PickUpItemRadius(ABaseCharacter* Character)
{
    if (bIsOverlap)
    {
        OnPickUp.Broadcast(Character);

        FoundGun = FindItemToPickUp();
        if (FoundGun)
        {
            FoundGun->AttachWeapon(Character); // later send to Inventory
            Destroy();
        }
    }
}