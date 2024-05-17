
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UProgressBar;
class ABaseCharacter;
class UTextBlock;

UCLASS()
class CODEZERO_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Intercation Widget")
	ABaseCharacter* PlayerReference;

protected:
	UPROPERTY()
	UTextBlock* NameText;

	UPROPERTY()
	UTextBlock* ActionText;

	UPROPERTY()
	UTextBlock* Quantity;

	UPROPERTY()
	UTextBlock* KeyPressText;

	//UProgressBar* 
};
