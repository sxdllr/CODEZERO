
#pragma once

#include "CoreMinimal.h"
#include "ItemDataBase.h"
//#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

class ABaseCharacter;

UCLASS()
class CODEZERO_API UItemBase : public UObject
{
	GENERATED_BODY()

	//UPROPERTY(meta = (UIMin = 1, UIMax = 128, AllowPrivateAccess = true))
	//UInventoryComponent* OwingInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 128, AllowPrivateAccess = true))
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	EITemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	FItemStats ItemStats;
	
	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	FItemAssetData AssetData;

public:
	UItemBase();

	UItemBase* CreateItemCopy() const;

	float GetItemStackSquare() const { return NumericData.ItemSquare; }
	bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; }

	void SetQuantity(const int32 NewQuantity);

	virtual void Use(ABaseCharacter* Character);

protected:
	bool operator==(const FName& OtherID) const { return ID == OtherID; }
};
