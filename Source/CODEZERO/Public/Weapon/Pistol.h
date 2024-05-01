
#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseGun.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class CODEZERO_API APistol : public ABaseGun
{
	GENERATED_BODY()

public:
	APistol();

private:
	virtual void OnFire() override;
};
