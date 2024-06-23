// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTION_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Selected(AController* SelectedBy);
	virtual void Selected_Implementation(AController* SelectedBy) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Unselected(AController* UnselectedBy);
	virtual void Unselected_Implementation(AController* UnselectedBy) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Use(AController* UsedBy);
	virtual void Use_Implementation(AController* UsedBy) = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AlternateUse(AController* AltUsedBy);
	virtual void AlternateUse_Implementation(AController* AltUsedBy) = 0;
};
