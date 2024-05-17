
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UENUM()
enum EInteractableType : uint8
{
	PickUp UMETA(DisplayName = "PickUp"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"),
	Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData()
		: InteractableType(EInteractableType::PickUp)
		, Name(FText::GetEmpty())
		, Action(FText::GetEmpty())
		, Quantity(0)
		, InteractionDuration(0.f)
	{
		
	};
	
	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<EInteractableType> InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class CODEZERO_API IInteractionInterface
{
	GENERATED_BODY()

public:
	FInteractableData InteractableData;

	virtual void OnEnterOverlap() = 0;
	virtual void OnInteract() = 0;
};
