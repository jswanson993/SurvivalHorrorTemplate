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
	UFUNCTION(BlueprintNativeEvent)
	void Selected();
	virtual void Selected_Implementation() = 0;
	UFUNCTION(BlueprintNativeEvent)
	void Unselected();
	virtual void Unselected_Implementation() = 0;
	UFUNCTION(BlueprintNativeEvent)
	void Use();
	virtual void Use_Implementation() = 0;
	UFUNCTION(BlueprintNativeEvent)
	void AlternateUse();
	virtual void AlternativeUse_Implementation() = 0;
};
