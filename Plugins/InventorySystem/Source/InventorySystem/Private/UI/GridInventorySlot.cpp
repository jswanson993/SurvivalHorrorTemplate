// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GridInventorySlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


UGridInventorySlot::UGridInventorySlot()
{
	
}

void UGridInventorySlot::SlotInfoUpdate()
{
	if(bIsMoving)
	{
		MovingIcon->SetVisibility(ESlateVisibility::Visible);
	}else
	{
		MovingIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(ItemSlot.Quantity == 0)
	{
		QuantityBorder->SetVisibility(ESlateVisibility::Hidden);
		QuantityText->SetVisibility(ESlateVisibility::Hidden);
		ItemIcon->SetBrushFromTexture(EmptySlotIcon);
		const FLinearColor OpaqueWhite = {255,255,255,.75};
		ItemIcon->SetColorAndOpacity(OpaqueWhite);
		bIsEmpty = true;
	}else
	{
		QuantityBorder->SetVisibility(ESlateVisibility::Visible);
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
		QuantityText->SetVisibility(ESlateVisibility::Visible);
		
		QuantityText->SetText(FText::FromString(FString::FromInt(ItemSlot.Quantity)));
		ItemIcon->SetBrushFromTexture(ItemSlot.Item.ItemIcon);
		ItemIcon->SetColorAndOpacity(FLinearColor::White);
		ItemName = ItemSlot.Item.ItemName;

		switch (ItemSlot.Item.ItemType)
		{
			case Consumable: EquipButton->SetVisibility(ESlateVisibility::Collapsed);
				break;
			case Equipment: UseButton->SetVisibility(ESlateVisibility::Collapsed);
				break;
			case KeyItem: EquipButton->SetVisibility(ESlateVisibility::Collapsed);
				DropButton->SetVisibility(ESlateVisibility::Collapsed);
				break;
			default:
				break;
		}
	}
}

void UGridInventorySlot::UpdateMovingImage(const EMoveWidgetState State)
{
	MovingIcon->SetDesiredSizeOverride(ItemIcon->GetDesiredSize());
	QuantityText->SetVisibility(ESlateVisibility::Hidden);
	switch(State)
	{
		case Clear:
			MovingIcon->SetColorAndOpacity(MovementClearColor);
	 		break;
		case Blocked:
			MovingIcon->SetColorAndOpacity(MovementBlockedColor);
			break;
		case Combine:
			MovingIcon->SetColorAndOpacity(MovementCombineColor);
			break;
	}
}

void UGridInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(UseButton != nullptr)
	{
		UseButton->OnClicked.AddDynamic(this, &UGridInventorySlot::OnUseButtonClicked);
	}
}


void UGridInventorySlot::NativeOnClicked()
{
	Super::NativeOnClicked();

	if(!(bIsEmpty || !bCanInteract))
	{
		switch(ItemOptionsBox->GetVisibility())
		{
			case ESlateVisibility::Visible:
				ItemOptionsBox->SetVisibility(ESlateVisibility::Hidden);
				break;
			case ESlateVisibility::Collapsed:
			case ESlateVisibility::Hidden:
				ItemOptionsBox->SetVisibility(ESlateVisibility::Visible);
				break;
			default:
				break;
		}
	}
}

void UGridInventorySlot::NativeOnHovered()
{
	Super::NativeOnHovered();

	if(!OnHovered.ExecuteIfBound(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("On Hovered Event Failed To Execute"));
	}
}

void UGridInventorySlot::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	switch(ItemOptionsBox->GetVisibility())
	{
		case ESlateVisibility::Visible:
			ItemOptionsBox->SetVisibility(ESlateVisibility::Hidden);
			break;
		default:
			break;
	}
}

void UGridInventorySlot::OnUseButtonClicked_Implementation()
{
	OnUse.ExecuteIfBound(this->ItemSlot.SlotId);
	ItemOptionsBox->SetVisibility(ESlateVisibility::Hidden);
	this->SetFocus();
}
