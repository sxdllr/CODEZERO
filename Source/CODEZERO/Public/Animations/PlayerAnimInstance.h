
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class CODEZERO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta=(AllowPrivateAccess = true))
	ABaseCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta=(AllowPrivateAccess = true))
	float Speed;
};
