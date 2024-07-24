#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum EItemType
{
	Consumable,
	Equipment,
	KeyItem
};

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItem
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemType> ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSingleTon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	FItem()
	{
		ItemName = TEXT("");
		Description = FText::GetEmpty();
		ItemType = EItemType::Consumable;
		ItemIcon = nullptr;
		ItemMesh = nullptr;
		bIsUsable = false;
		bIsSingleTon = false;
		MaxQuantity = 0;
		ActorClass = nullptr;
	}
	
};
