
#include "Animations/PlayerAnimInstance.h"
#include "Character/BaseCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	: Speed(0.f)
	, Character(nullptr)
{}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	if(!Character) { return; }

	Speed = Character->GetVelocity().Size();
}
