
#include "Character/BaseCharacter.h"
#include "Inventory/ComponentPicker.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ABaseCharacter::ABaseCharacter()
	: CameraBoom(nullptr)
	, FollowCamera(nullptr)
	, Mesh3P(nullptr)
	, Mesh1P(nullptr)
	, DefaultMappingContext(nullptr)
	, LookAction(nullptr)
	, MoveAction(nullptr)
	, SprintAction(nullptr)
	, PickAction(nullptr)
	, Camera1P(nullptr)
	, bHasGun(false)
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.0f, 0.f);
	
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetupAttachment(RootComponent);
	Mesh3P->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	Mesh3P->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	Mesh3P->SetOnlyOwnerSee(true);
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Mesh3P);
	CameraBoom->TargetArmLength = MaxZoomLength;
	CameraBoom->SetWorldLocation(FVector(0.f, 30.f, 150.f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
	FollowCamera->bUsePawnControlRotation = false;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(Mesh3P);
	Mesh1P->SetRelativeLocation(FVector(250.f, -20.f, 0.f));
	Mesh1P->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ABaseCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::None, this, &ABaseCharacter::StopSprint);

		EnhancedInputComponent->BindAction(PickAction, ETriggerEvent::Triggered, this, &ABaseCharacter::PickUp);

		EnhancedInputComponent->BindAction(Camera1P, ETriggerEvent::Triggered, this, &ABaseCharacter::Aim);
		EnhancedInputComponent->BindAction(Camera1P, ETriggerEvent::None, this, &ABaseCharacter::StopAim);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}


void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ABaseCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABaseCharacter::PickUp()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AComponentPicker::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (AComponentPicker* ComponentPicker = Cast<AComponentPicker>(Actor))
		{
			ComponentPicker->PickUpItemRadius(this);
			return;
		}
	}
}

void ABaseCharacter::Aim()
{
	if(bHasGun)
	{
		CameraBoom->TargetArmLength -= ZoomSpeed * GetWorld()->GetDeltaSeconds();
		if (CameraBoom->TargetArmLength <= MinZoomLength)
		{
			CameraBoom->TargetArmLength = MinZoomLength;
		}
	}
}

void ABaseCharacter::StopAim()
{
	CameraBoom->TargetArmLength += ZoomSpeed * GetWorld()->GetDeltaSeconds();
	if (CameraBoom->TargetArmLength >= MaxZoomLength)
	{
		CameraBoom->TargetArmLength = MaxZoomLength;
	}
}