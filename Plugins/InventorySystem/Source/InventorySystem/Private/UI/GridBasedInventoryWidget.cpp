// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GridBasedInventoryWidget.h"

#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Spatial/PointHashGrid2.h"
#include "UI/GridInventorySlot.h"

void UGridBasedInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bIsMoving)
	{
		Move();
	}
}

bool UGridBasedInventoryWidget::Add_Implementation(FSlot InventorySlot)
{
	FVector2D Placement;
	if(!FindSlotPlacement(InventorySlot,Placement))
	{
		return false;
	}

	UGridInventorySlot* NewSlotWidget = CreateWidget<UGridInventorySlot>(this, SlotWidget);
	NewSlotWidget->SetItemSlot(InventorySlot);
	NewSlotWidget->OnHovered.BindDynamic(this, &UGridBasedInventoryWidget::UpdateHoveredSlot);
	NewSlotWidget->OnUse.BindDynamic(this, &UGridBasedInventoryWidget::OnItemUsed);

	UGridSlot* PanelSlot = SlotPanel->AddChildToGrid(NewSlotWidget, Placement.Y, Placement.X);
	PanelSlot->SetPadding(SlotPadding);
	PanelSlot->SetRowSpan(InventorySlot.Size.Y);
	PanelSlot->SetColumnSpan(InventorySlot.Size.X);

	InventorySlots.Add(InventorySlot.SlotId, GetIndexFromCoordinates(Placement));

	for(int Column = Placement.X; Column < InventorySlot.Size.X + Placement.X; Column++)
	{
		for(int Row = Placement.Y; Row < InventorySlot.Size.Y + Placement.Y; Row++)
		{
			const FVector2D Coordinate(Column, Row);
			const int Index = GetIndexFromCoordinates(Coordinate);
			SlotArray[Index]->RemoveFromParent();
			SlotArray[Index] = NewSlotWidget;
		}
	}

	NewSlotWidget->SlotInfoUpdate();
	return true;
}

bool UGridBasedInventoryWidget::Update_Implementation(FSlot InventorySlot)
{
	if(InventorySlots.IsEmpty())
	{
		return Add(InventorySlot);;
	}

	if(!InventorySlots.Contains(InventorySlot.SlotId))
	{
		return false;
	}
	const int SlotIndex = *InventorySlots.Find(InventorySlot.SlotId);
	if(SlotIndex > SlotArray.Num())
	{
		return false;
	}
	
	UGridInventorySlot* ExistingSlot = SlotArray[SlotIndex];
	if(ExistingSlot == nullptr)
	{
		return false;
	}
	
	ExistingSlot->SetItemSlot(InventorySlot);
	ExistingSlot->SlotInfoUpdate();
	return true;
}

bool UGridBasedInventoryWidget::Remove_Implementation(FSlot InventorySlot)
{
	return false;
}

void UGridBasedInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitializeOpenSlots();
	for(UGridInventorySlot* InventorySlot : SlotArray)
	{
		InventorySlot->OnHovered.BindDynamic(this, &UGridBasedInventoryWidget::UpdateHoveredSlot);
	}
}

void UGridBasedInventoryWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	this->GetDesiredFocusTarget()->SetFocus();
}

UWidget* UGridBasedInventoryWidget::NativeGetDesiredFocusTarget() const
{
	UWidget* FocusWidget = Super::NativeGetDesiredFocusTarget();

	if(SlotArray.Num() > 0)
	{
		FocusWidget = SlotArray[0];
	}

	return FocusWidget;
}


void UGridBasedInventoryWidget::OnItemUsed_Implementation(FGuid SlotId)
{
	OnUseItem.Broadcast(SlotId);
}

void UGridBasedInventoryWidget::OnSlotClicked_Implementation(UGridInventorySlot* Button)
{
	if(bIsMoving)
	{
		FinishMove();
		for(UGridInventorySlot* InventorySlot : SlotArray)
		{
			InventorySlot->SetIsInteractionEnabled(true);
		}
	}
}

bool UGridBasedInventoryWidget::FindSlotPlacement_Implementation(const FSlot InventorySlot, FVector2D& OutPlacement)
{
	bool FoundSpace = false;
	for(int SlotIndex = 0; SlotIndex < SlotArray.Num(); SlotIndex++)
	{
		FoundSpace = true;
		OutPlacement = GetCoordinatesFromIndex(SlotIndex);
		const int ColumnStartIndex = OutPlacement.X;
		const int ColumnEndIndex = OutPlacement.X + InventorySlot.Size.X;
		const int RowStartIndex = OutPlacement.Y;
		const int RowEndIndex = OutPlacement.Y + InventorySlot.Size.Y;
		for(int Column = ColumnStartIndex; Column < ColumnEndIndex; Column++)
		{
			if(Column >= MaxWidth)
			{
				FoundSpace = false;
				break;
			}

			for(int Row = RowStartIndex; Row < RowEndIndex; Row++)
			{
				if(Row >= MaxHeight)
				{
					FoundSpace = false;
					break;
				}
				const FVector2D GraphCoordinate(Column, Row);
				const int SlotWidgetIndex = GetIndexFromCoordinates(GraphCoordinate);
				if(!SlotArray[SlotWidgetIndex]->GetIsEmpty())
				{
					FoundSpace = false;
					break;
				}
			}

			if(!FoundSpace)
			{
				break;
			}
		}

		if(FoundSpace)
		{
			break;
		}
	}
	
	return FoundSpace;
}

void UGridBasedInventoryWidget::InitializeOpenSlots()
{
	if(SlotWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Slot Widget Not Set"))
		return;
	}
	for(int i = 0; i < MaxHeight; i++)
	{
		for(int j = 0; j < MaxWidth; j++)
		{
			auto NewEmptySlot = CreateWidget<UGridInventorySlot>(this, SlotWidget);
			if(NewEmptySlot == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Error: Could Not Create Slot From Slot Widget"))
				return;
			}
			NewEmptySlot->SetItemSlot({});
			NewEmptySlot->SetIsEmpty(true);
			NewEmptySlot->AddToViewport();

			AddSlotToGrid(NewEmptySlot, i, j, 1, 1);
			NewEmptySlot->SlotInfoUpdate();
			NewEmptySlot->OnHovered.BindDynamic(this, &UGridBasedInventoryWidget::UpdateHoveredSlot);
		}
	}
}

void UGridBasedInventoryWidget::AddSlotToGrid(UGridInventorySlot* NewSlot, int Row, int Column, int RowSpan,
	int ColumnSpan, bool bAddPadding, bool bAddToSlotArray)
{
	UGridSlot* GridSlot = SlotPanel->AddChildToGrid(NewSlot, Row, Column);
	GridSlot->SetRowSpan(RowSpan);
	GridSlot->SetColumnSpan(ColumnSpan);
	if(bAddPadding)
	{
		GridSlot->SetPadding(SlotPadding);
	}

	if(bAddToSlotArray)
	{
		SlotArray.Add(NewSlot);
	}
}

void UGridBasedInventoryWidget::HandleInput(FName InputName)
{
	if (InputName == TEXT("Move"))
	{
		if(bIsMoving == false)
		{
			StartMove();
			for(UGridInventorySlot* InventorySlot : SlotArray)
			{
				InventorySlot->SetIsInteractionEnabled(false);
			}
		}else
		{
			FinishMove();
		}

		for(UGridInventorySlot* InventorySlot : SlotArray)
		{
			InventorySlot->OnClicked.BindDynamic(this, &UGridBasedInventoryWidget::OnSlotClicked);
		}
	}
}

FVector2D UGridBasedInventoryWidget::GetCoordinatesFromIndex(const int Index)
{
	FVector2D Coordinates;
	Coordinates.Y = (int)(Index / MaxWidth);
	Coordinates.X = (int)(Index % MaxWidth);

	return Coordinates;
}

int UGridBasedInventoryWidget::GetIndexFromCoordinates(const FVector2D Coordinate)
{
	return (Coordinate.Y * MaxWidth) + Coordinate.X;
}

int GetExtent(int Index, int Span)
{
	return Index + Span;
}

void UGridBasedInventoryWidget::StartMove()
{
	if(HoveredSlot->GetIsEmpty())
	{
		return;
	}

	bIsMoving = true;

	MovingSlot = CreateWidget<UGridInventorySlot>(this, SlotWidget);
	MovingSlot->SetItemSlot(HoveredSlot->GetItemSlot());
	MovingSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
	MovingSlot->SlotInfoUpdate();
	MovingSlot->AddToViewport();
	
	const FVector2D Coordinate = GetCoordinatesFromIndex(SlotArray.Find(HoveredSlot));
	UGridSlot* GridSlot = SlotPanel->AddChildToGrid(MovingSlot,  Coordinate.Y,  Coordinate.X);
	GridSlot->SetLayer(1);
	GridSlot->SetRowSpan(MovingSlot->GetItemSlot().Size.Y);
	GridSlot->SetColumnSpan(MovingSlot->GetItemSlot().Size.X);
	
	OriginalSlot = HoveredSlot;
}

void UGridBasedInventoryWidget::FinishMove()
{
	UGridSlot* MovingGridSlot;
	UGridSlot* OriginalGridSlot;
	switch(MoveMode)
	{
	case Clear:
		OriginalGridSlot = Cast<UGridSlot>(OriginalSlot->Slot); 
		SlotPanel->RemoveChild(OriginalSlot);
		for(int Row = OriginalGridSlot->GetRow(); Row < GetExtent(OriginalGridSlot->GetRow(), OriginalGridSlot->GetRowSpan()); Row++)
		{
			for(int Column = OriginalGridSlot->GetColumn(); Column < GetExtent(OriginalGridSlot->GetColumn(), OriginalGridSlot->GetColumnSpan()); Column++)
			{
				const FVector2D Coordinate(Column, Row);
				if(SlotArray[GetIndexFromCoordinates(Coordinate)]->GetParent() != nullptr)
				{
					continue;
				}

				auto NewEmptySlot = CreateWidget<UGridInventorySlot>(this, SlotWidget);
				NewEmptySlot->SetIsEmpty(true);
				NewEmptySlot->SlotInfoUpdate();
				NewEmptySlot->AddToViewport();
				NewEmptySlot->OnHovered.BindDynamic(this, &UGridBasedInventoryWidget::UpdateHoveredSlot);
			
				UGridSlot* GridSlot = SlotPanel->AddChildToGrid(NewEmptySlot, OriginalGridSlot->GetRow(), OriginalGridSlot->GetColumn());
				GridSlot->SetPadding(SlotPadding);
				SlotArray[GetIndexFromCoordinates(Coordinate)] = NewEmptySlot;
			}
		}
		MovingGridSlot = Cast<UGridSlot>(MovingSlot->Slot);
		for(int Row = MovingGridSlot->GetRow(); Row < GetExtent(MovingGridSlot->GetRow(), MovingGridSlot->GetRowSpan()); Row++)
		{
			for(int Column = MovingGridSlot->GetColumn(); Column < GetExtent(MovingGridSlot->GetColumn(),  MovingGridSlot->GetColumnSpan()); Column++)
			{
				const FVector2D Coordinates(Column, Row);
				const int SlotIndex = GetIndexFromCoordinates(Coordinates);
				SlotArray[SlotIndex]->RemoveFromParent();
				SlotArray[SlotIndex] = MovingSlot;
				InventorySlots.Add(OriginalSlot->GetItemSlot().SlotId, SlotIndex);
			}
		}
		MovingGridSlot->SetLayer(0);
		MovingGridSlot->SetPadding(SlotPadding);
		MovingSlot->SetIsMoving(false);
		MovingSlot->SetVisibility(ESlateVisibility::Visible);
		MovingSlot->SlotInfoUpdate();
		MovingSlot->OnHovered.BindDynamic(this, &UGridBasedInventoryWidget::UpdateHoveredSlot);
		MovingSlot->OnUse.BindDynamic(this, &UGridBasedInventoryWidget::OnItemUsed);
		HoveredSlot = MovingSlot;
		MovingSlot = nullptr;
		OriginalSlot = nullptr;
		bIsMoving = false;
		break;
	case Combine:
		OnCombineSlots.Broadcast(MovingSlot->GetItemSlot().SlotId, HoveredSlot->GetItemSlot().SlotId);
	case Blocked:
		MovingSlot->RemoveFromParent();
		MovingSlot = nullptr;
		OriginalSlot = nullptr;
		bIsMoving = false;
		break;
	}
}

void UGridBasedInventoryWidget::Move()
{
	const UGridSlot* HoveredGridSlot = Cast<UGridSlot>(HoveredSlot->Slot);
	const int Row = HoveredGridSlot->GetRow();
	const int Column = HoveredGridSlot->GetColumn();
	if(((MovingSlot->GetItemSlot().Size.Y + Row) > MaxHeight) ||
		((MovingSlot->GetItemSlot().Size.X + Column) > MaxWidth))
	{
		return;
	}

	UGridSlot* GridSlot = Cast<UGridSlot>(MovingSlot->Slot);
	if(GridSlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Get Grid Slot From Moving Slot"));
		return;
	}

	GridSlot->SetRow(Row);
	GridSlot->SetColumn(Column);
	const int RowExtent = Row + MovingSlot->GetItemSlot().Size.Y - 1;
	const int ColumnExtent = Column + MovingSlot->GetItemSlot().Size.X -1;
	if(HoveredSlot->GetIsEmpty())
	{
		for(int i = Row; i <= RowExtent; i++)
		{
			for(int j = Column; j <= ColumnExtent; j++)
			{
				const FVector2D SlotCoordinate(j, i);
				if(SlotArray[GetIndexFromCoordinates(SlotCoordinate)]->GetIsEmpty())
				{
					MoveMode = EMoveWidgetState::Clear;
				}else
				{
					MoveMode = EMoveWidgetState::Blocked;
				}
			}
		}
	}else if(HoveredSlot == OriginalSlot)
	{
		MoveMode = EMoveWidgetState::Clear;
	}else if(HoveredSlot->GetItemName().Equals(MovingSlot->GetItemName()))
	{
		MoveMode = EMoveWidgetState::Combine;
	}else
	{
		MoveMode = EMoveWidgetState::Blocked;
	}

	MovingSlot->UpdateMovingImage(MoveMode);
}


