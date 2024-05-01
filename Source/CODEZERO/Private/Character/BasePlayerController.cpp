
#include "Character/BasePlayerController.h"
#include "EnhancedInputSubsystems.h"

ABasePlayerController::ABasePlayerController()
	: InputMappingContext(nullptr) {}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
