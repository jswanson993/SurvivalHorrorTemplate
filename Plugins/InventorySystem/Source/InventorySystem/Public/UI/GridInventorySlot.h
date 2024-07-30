// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Utilities/Slot.h"
#include "GridInventorySlot.generated.h"

UENUM(BlueprintType)
enum EMoveWidgetState
{
	Clear,
	Blocked,
	Combine
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHoveredDelegate, UGridInventorySlot*, InventorySlot);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUseDelegate, FGuid, SlotId);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnClickedDelegate, UGridInventorySlot*, InventorySlot);

class UTextBlock;
class UVerticalBox;
class UBorder;
class USizeBox;
class UImage;
/**
 * This class focuses primarily on the logic of the Slot Widget and not the overall layout.
 * To use: create a widget blueprint and make sure that each of the subwidgets listed are present before making
 *	this class the parent
 */
UCLASS()
class INVENTORYSYSTEM_API UGridInventorySlot : public UCommonButtonBase
{
	GENERATED_BODY()

	//SubWidgets
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UImage* ItemIcon;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UImage* MovingIcon;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UBorder* QuantityBorder;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UTextBlock* QuantityText;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* ItemOptionsBox;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UButton* EquipButton;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UButton* InspectButton;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess="true"))
	UButton* DropButton;

	//Defaults
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UTexture2D* EmptySlotIcon;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	FLinearColor MovementClearColor = {1, 1, 1, .75};
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	FLinearColor MovementBlockedColor = {1, 0, 0, .75};
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	FLinearColor MovementCombineColor = {0, 0, 1, .75};

	//Variables
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FString ItemName;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FSlot ItemSlot;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsEmpty = false;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bCanInteract;
	

public:

	UGridInventorySlot();
	
	FOnHoveredDelegate OnHovered;
	FOnUseDelegate OnUse;
	FOnClickedDelegate OnClicked;

	UFUNCTION(BlueprintSetter)
	void SetItemName(FString Name) { ItemName = Name; }
	UFUNCTION(BlueprintSetter)
	void SetItemSlot(const FSlot& SlotData) { ItemSlot = SlotData; };
	UFUNCTION(BlueprintSetter)
	void SetIsMoving(const bool IsMoving) { bIsMoving = IsMoving; }
	UFUNCTION(BlueprintSetter)
	void SetCanInteract(const bool CanInteract) { bCanInteract = CanInteract; } 
	UFUNCTION(BlueprintSetter)
	void SetIsEmpty(const bool Empty) { bIsEmpty = Empty; }
	UFUNCTION(BlueprintGetter)
	bool GetIsEmpty() const { return bIsEmpty; }
	UFUNCTION(BlueprintGetter)
	FSlot GetItemSlot() const { return ItemSlot; }
	UFUNCTION(BlueprintGetter)
	FString GetItemName() const { return ItemName; }

	/*
	 * Updates all visual components of slot to reflect the ItemSlot values.
	 * Required for visual changes to be updated after the ItemSlot is updated.
	 */
	void SlotInfoUpdate();

	void UpdateMovingImage(const EMoveWidgetState State);
	
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnClicked() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	UFUNCTION(BlueprintNativeEvent)
	void OnUseButtonClicked();
	virtual void OnUseButtonClicked_Implementation();
	
};
