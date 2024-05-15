
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataBase.generated.h"

UENUM()
enum class EITemType : uint8
{
	Melee UMETA(DisplayName = "Knife"),
	Fire UMETA(DisplayName = "Weapon"),
	Throw UMETA(DisplayName = "Grenade"),
	Ammo UMETA(DisplayName = "Ammo"),
	Heal UMETA(DisplayName = "Heal"),
	Quest UMETA(DisplayName = "QuestItem")
};

USTRUCT()
struct FItemStats
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float DamageValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	FText Description;
	
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	
	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float ItemSquare;

	UPROPERTY(EditAnywhere)
	int16 MaxStackSize;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable;

	UPROPERTY(EditAnywhere)
	bool bCanDrop;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemDataBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EITemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Stats")
	FItemStats ItemStats;

	UPROPERTY(EditAnywhere, Category = "Text Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Numeric Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Asset Data")
	FItemAssetData AssetData;
};
