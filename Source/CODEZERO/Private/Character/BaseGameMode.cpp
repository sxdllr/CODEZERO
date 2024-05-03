
#include "Character/BaseGameMode.h"

ABaseGameMode::ABaseGameMode()
	:Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Main/Blueprints/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<AController> PlayerControllerClassFinder(TEXT("/Game/Main/Blueprints/Character/BP_PlayerController"));
	if (PlayerControllerClassFinder.Succeeded())
	{
		PlayerControllerClass = PlayerControllerClassFinder.Class;
	}
}