// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utilities/Inventory.h"
#include "Components/ActorComponent.h"
#include "Utilities/Slot.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryUpdatedDelegate, FSlot, ItemSlot);


USTRUCT()
struct FItemTotal
{
	GENERATED_USTRUCT_BODY()
	FItem Item = FItem();
	int Quantity = 0;
	bool FoundItem = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent, public IInventory
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<FGuid, FSlot> ItemSlots;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	int InventorySize;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UUserWidget* InventoryWidget;
	FInventoryUpdatedDelegate InventoryUpdatedDelegate;
	
public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual int AddToInventoryWidget(int Quantity, FItem Item, FVector2D Size);
	UFUNCTION(BlueprintCallable)
	virtual void UpdateInventoryWidget(TArray<FSlot> SlotsToUpdate);
	UFUNCTION(BlueprintCallable)
	virtual bool CheckSlotOverMaxQuantity(const FSlot Slot,const int NewQuantity, int& Overflow);
	UFUNCTION(BlueprintCallable)
	virtual FVector2D GetItemSize(const FItem Item);
	UFUNCTION(BlueprintCallable)
	virtual void CombineSlots(const FGuid SlotId1, const FGuid SlotId2);
public:	
	// Called every frameFItem GetItem_Implementation(const FString ItemName)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool AddItem_Implementation(FItem Item, int Quantity, int& QuantityRemaining) override;
	virtual bool UseItem_Implementation(FGuid ItemId) override;
	virtual bool GetItem_Implementation(const FString& ItemName, FItem& OutItem, int& OutQuantity) override;
	virtual bool DropItem_Implementation(FGuid ItemId, int Quantity) override;
	virtual bool EquipItem_Implementation(FGuid ItemId) override;
	virtual bool InspectItem_Implementation(FGuid ItemId) override;

	UFUNCTION(BlueprintCallable)
	TArray<FSlot> GetSlots();
	UFUNCTION(BlueprintCallable)
	TArray<FGuid> FindInventoryItems(const FItem Item);
	UFUNCTION(BlueprintCallable)
	static void UpdateSlotQuantity(FSlot& Slot, const int Quantity);
private:
	static bool CompareInventoryItems(const FItem& Item1, const FItem& Item2);
	
};
