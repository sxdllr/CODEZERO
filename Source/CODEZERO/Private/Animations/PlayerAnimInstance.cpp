
#include "Animations/PlayerAnimInstance.h"
#include "Character/BaseCharacter.h"
//#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	: Character(nullptr)
	, Speed(0.f)
{}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if(!Character) { return; }

	FVector Velocity{ Character->GetVelocity().Size() };
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
}
