
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UMainMenu;
class UInteractionWidget;

UCLASS()
class CODEZERO_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = true))
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

public:
	ABaseHUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget() const;
	
	bool bIsMenuVisible;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;
};
