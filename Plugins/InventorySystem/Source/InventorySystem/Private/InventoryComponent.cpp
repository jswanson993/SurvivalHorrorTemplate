// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Utilities/InventoryItem.h"
#include "Utilities/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

int UInventoryComponent::AddToInventoryWidget(int Quantity, FItem Item, FVector2D Size)
{
	FGuid SlotId;
	do
	{
		SlotId = FGuid::NewGuid();
	}while(ItemSlots.Contains(SlotId));

	FSlot Slot{
		.SlotId = SlotId,
		.Item = Item,
		.Size = Size
	};
	int Overflow = 0;
	CheckSlotOverMaxQuantity(Slot, Quantity, Overflow);
	Slot.Quantity = Quantity - Overflow;
	const bool Success = IInventoryWidget::Execute_Add(InventoryWidget, Slot);
	if(Success)
	{
		ItemSlots.Add(SlotId, Slot);
		UE_LOG(LogTemp, Display, TEXT("Successfully Added to Inventory"));
		return Overflow;
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Add To Inventory"));
		return 0;
	}
	
}

void UInventoryComponent::UpdateInventoryWidget(TArray<FSlot> SlotsToUpdate)
{
	for(const FSlot Slot : SlotsToUpdate)
	{
		IInventoryWidget::Execute_Update(InventoryWidget, Slot);
	}
}

bool UInventoryComponent::CheckSlotOverMaxQuantity(const FSlot Slot, const int NewQuantity, int& Overflow)
{
	const int Quantity = Slot.Quantity + NewQuantity;
	if(Quantity > Slot.Item.MaxQuantity)
	{
		Overflow = FMath::Max(Quantity - Slot.Item.MaxQuantity, 0);
		return true;
	}

	Overflow = 0;
	return false;
}

FVector2D UInventoryComponent::GetItemSize(const FItem Item)
{
	const float SizeX = Item.ItemIcon->GetSizeX();
	const float SizeY = Item.ItemIcon->GetSizeY();
	
	const float Width = FMath::CeilToInt(SizeX/SizeY);
	const float Height = FMath::CeilToInt(SizeY/SizeX);

	const FVector2D Size = {Width, Height};

	return Size;
}

void UInventoryComponent::CombineSlots(const FGuid SlotId1, const FGuid SlotId2)
{
	if(!ItemSlots.Contains(SlotId1) && ItemSlots.Contains(SlotId2))
	{
		return;
	}
	FSlot* Slot1 = ItemSlots.Find(SlotId1);
	FSlot* Slot2 = ItemSlots.Find(SlotId2);
	int Overflow = 0;
	if(CheckSlotOverMaxQuantity(*Slot2, Slot1->Quantity, Overflow))
	{
		Slot2->Quantity = Slot2->Item.MaxQuantity;
	}else
	{
		UpdateSlotQuantity(*Slot2, Slot1->Quantity);
	}

	Slot1->Quantity = Overflow;
	
	IInventoryWidget::Execute_Update(InventoryWidget, *Slot1);
	IInventoryWidget::Execute_Update(InventoryWidget, *Slot2);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem_Implementation(FItem Item, int Quantity, int& QuantityRemaining)
{
	TArray<FSlot> SlotsToUpdate;
	QuantityRemaining = Quantity;
	TArray<FGuid> ExistingSlots = FindInventoryItems(Item);
	if(ExistingSlots.IsEmpty())
	{
		while(QuantityRemaining > 0)
		{
			QuantityRemaining = AddToInventoryWidget(QuantityRemaining, Item, GetItemSize(Item));
		}
		return true;
	}

	for(FGuid SlotId : ExistingSlots)
	{
		if(QuantityRemaining == 0)
		{
			break;
		}
		if(!ItemSlots.Contains(SlotId) || !CompareInventoryItems(ItemSlots.Find(SlotId)->Item, Item))
		{
			continue;
		}
		FSlot* Slot = ItemSlots.Find(SlotId);
		int Overflow;
		const bool bIsOverMaxQuantity = CheckSlotOverMaxQuantity(*Slot, QuantityRemaining, Overflow);
		if(bIsOverMaxQuantity && Item.bIsSingleton)
		{
			Overflow = 0;
		}
		const int NewQuantity = QuantityRemaining - Overflow;
		if(NewQuantity > 0)
		{
			UpdateSlotQuantity(*Slot, NewQuantity);
			SlotsToUpdate.Add(*Slot);
			QuantityRemaining = Overflow;
		}
	}
	while(QuantityRemaining != 0)
	{
		QuantityRemaining = AddToInventoryWidget(QuantityRemaining, Item, GetItemSize(Item));
	}

	UpdateInventoryWidget(SlotsToUpdate);
	return true;
}

bool  UInventoryComponent::UseItem_Implementation(FGuid ItemId)
{
	if(!ItemSlots.Contains(ItemId))
	{
		return false;
	}
	FSlot* Slot = ItemSlots.Find(ItemId);
	if(Slot->Item.ActorClass->ImplementsInterface(UInventoryItem::StaticClass()))
	{
		AActor* Item = GetWorld()->SpawnActor(Slot->Item.ActorClass);
		const APawn* Owner = Cast<APawn>(this->GetOwner());
		if(Item == nullptr || Owner == nullptr)
		{
			return false;
		}
		Item->SetActorHiddenInGame(true);
		
		const bool Consume = IInventoryItem::Execute_UseItem(Item, Owner->GetController());
		Item->Destroy();
		if(Consume)
		{
			Slot->Quantity --;
			return IInventoryWidget::Execute_Update(InventoryWidget, *Slot);
		}
	}
	return true;
}

bool UInventoryComponent::GetItem_Implementation(const FString& ItemName, FItem& OutItem, int& OutQuantity)
{
	FItem Item = FItem();
	Item.ItemName = ItemName;
	TArray<FGuid> SlotIds =  FindInventoryItems(Item);

	if(SlotIds.IsEmpty())
	{
		OutItem = FItem();
		OutQuantity = 0;
		return false;
	}

	int TotalQuantity = 0;
	for(FGuid Id : SlotIds)
	{
		if(!ItemSlots.Contains(Id))
		{
			continue;
		}
		FSlot* Slot = ItemSlots.Find(Id);
		OutItem = Slot->Item;
		TotalQuantity += Slot->Quantity;
	}

	OutQuantity = TotalQuantity;
	return true;
}

bool UInventoryComponent::DropItem_Implementation(FGuid ItemId, int Quantity)
{
	if(!ItemSlots.Contains(ItemId))
	{
		return false;
	}

	FSlot* Slot = ItemSlots.Find(ItemId);
	
	const int NewQuantity = Slot->Quantity - Quantity;
	if(NewQuantity <= 0)
	{
		const FSlot EmptySlot
		{
			.SlotId = ItemId,
			.Item = FItem(),
			.Quantity = 0,
			.Size = {0,0}
		};
		ItemSlots.Remove(ItemId);
		InventoryUpdatedDelegate.Broadcast(EmptySlot);
		return true;
	}else
	{
		Slot->Quantity = NewQuantity;
		InventoryUpdatedDelegate.Broadcast(*Slot);
		return true;
	}
}

bool UInventoryComponent::EquipItem_Implementation(FGuid ItemId)
{
	return false;
}

bool UInventoryComponent::InspectItem_Implementation(FGuid ItemId)
{
	return false;
}

TArray<FSlot> UInventoryComponent::GetSlots()
{
	TArray<FSlot> Slots;
	ItemSlots.GenerateValueArray(Slots);
	return Slots;
}

TArray<FGuid> UInventoryComponent::FindInventoryItems(const FItem Item)
{
	TArray<FGuid> ExistingItems;
	for(auto Slot : ItemSlots)
	{
		if(Slot.Value.Item.ItemName.Equals(Item.ItemName))
		{
			ExistingItems.Add(Slot.Key);
		}
	}

	return ExistingItems;
}

void UInventoryComponent::UpdateSlotQuantity(FSlot& Slot, const int Quantity)
{
	Slot.Quantity += Quantity;
}

bool UInventoryComponent::CompareInventoryItems(const FItem& Item1, const FItem& Item2)
{
	if(Item1.ItemName.Equals(Item2.ItemName))
	{
		return true;
	}

	return false;
}

