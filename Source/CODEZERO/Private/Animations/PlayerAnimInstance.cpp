
#include "Animations/PlayerAnimInstance.h"
#include "Character/BaseCharacter.h"
//#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	: Character(nullptr)
	, Speed(0.f)
	, MovementOffsetYaw(0.f)
{}

void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaSeconds)
{
	if(!Character)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	}

	if(Character)
	{
		FVector Velocity{ Character->GetVelocity().Size() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();

		/*
		const FRotator AimRotation = Character->GetBaseAimRotation();
		const FRotator MovementRotator = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotator, AimRotation).Yaw;
		*/
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	Character = Cast<ABaseCharacter>(TryGetPawnOwner());
}
