// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtendedCommonUI/Public/ExtendedCommonActivatableWidget.h"
#include "Utilities/InventoryWidget.h"
#include "GridBasedInventoryWidget.generated.h"

class UExtendedCommonActivatableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombinedSlotsDelegate, FGuid, Slot1, FGuid, Slot2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemUsedDelegate, FGuid, SlotId);


enum EMoveWidgetState : int;
class UGridInventorySlot;
class UGridPanel;
class UCommonBoundActionBar;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UGridBasedInventoryWidget : public UExtendedCommonActivatableWidget, public IInventoryWidget
{
	
	GENERATED_BODY()
	
	//Subwidgets
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UGridPanel* SlotPanel;

	//Defaults
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int MaxSlots = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int MaxWidth = 8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int MaxHeight = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FMargin SlotPadding = {5};
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGridInventorySlot> SlotWidget;
	
	//Variables
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TArray<UGridInventorySlot*> SlotArray;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TMap<FGuid, int> InventorySlots; //Key: Slot Id, Value: Slot Array Index
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bIsMoving = false;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UGridInventorySlot* MovingSlot;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UGridInventorySlot* HoveredSlot;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UGridInventorySlot* OriginalSlot;

	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	TArray<FDataTableRowHandle> Inputs;
	FInputActionExecutedDelegate InputActionExecutedDelegate;
	
	EMoveWidgetState MoveMode;
	FVector2D HoveredCoordinate = {0, 0};

	

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnItemUsed(FGuid SlotId);
	virtual void OnItemUsed_Implementation(FGuid SlotId);

	UFUNCTION(BlueprintNativeEvent)
	void OnSlotClicked(UGridInventorySlot* Button);
	virtual void OnSlotClicked_Implementation(UGridInventorySlot* Button);

	UFUNCTION(BlueprintCallable)
	void StartMove();
	UFUNCTION(BlueprintCallable)
	void FinishMove();
	UFUNCTION(BlueprintCallable)
	void Move();

	UFUNCTION(BlueprintNativeEvent)
	bool FindSlotPlacement(const FSlot InventorySlot, FVector2D& OutPlacement);
	virtual bool FindSlotPlacement_Implementation(const FSlot InventorySlot, FVector2D& OutPlacement);

	
private:

	void InitializeOpenSlots();

	void AddSlotToGrid(UGridInventorySlot* NewSlot, int Row, int Column, int RowSpan, int ColumnSpan,
		bool bAddPadding = true, bool bAddToSlotArray = true);

	UFUNCTION()
	void UpdateHoveredSlot(UGridInventorySlot* NewHoveredSlot) { HoveredSlot = NewHoveredSlot; };

	UFUNCTION()
	void HandleInput(FName InputName);

	UFUNCTION(BlueprintCallable)
	FVector2D GetCoordinatesFromIndex(const int Index);
	UFUNCTION(BlueprintCallable)
	int GetIndexFromCoordinates(const FVector2D Coordinate);
	

public:

	UPROPERTY(BlueprintAssignable, meta=(DisplayName="Combine Slots Dispatcher"))
	FCombinedSlotsDelegate OnCombineSlots;
	
	UPROPERTY(BlueprintAssignable, meta=(DisplayName="Item Used Dispatcher"))
	FItemUsedDelegate OnUseItem;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual bool Add_Implementation(FSlot InventorySlot) override;
	virtual bool Update_Implementation(FSlot InventorySlot) override;
	virtual bool Remove_Implementation(FSlot InventorySlot) override;

	UFUNCTION(BlueprintCallable)
	EMoveWidgetState GetMoveMode() const { return MoveMode; }
	UFUNCTION(BlueprintCallable)
	void SetMoveMode(const EMoveWidgetState NewState) { MoveMode = NewState; }
};
