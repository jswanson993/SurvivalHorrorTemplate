// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item.h"
#include "Slot.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FGuid SlotId = {};
	UPROPERTY(EditAnywhere)
	FItem Item = FItem();
	UPROPERTY(EditAnywhere)
	int Quantity = 0;
	UPROPERTY(EditAnywhere)
	FVector2D Size = {0, 0};
};
