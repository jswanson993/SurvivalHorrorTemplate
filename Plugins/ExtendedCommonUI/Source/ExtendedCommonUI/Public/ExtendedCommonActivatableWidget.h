// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ExtendedCommonActivatableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);

USTRUCT(BlueprintType)
struct FInputActionBindingHandle
{
	GENERATED_BODY()

public:
	FUIActionBindingHandle Handle;
};

/**
 * Extends UCommonActivatableWidget with Blueprint-visible functions for registering additional input action bindings.
 *
 * Source From: https://unrealist.org/commonui-actionbar/
 */
UCLASS(meta=(DisableNativeTick))
class EXTENDEDCOMMONUI_API UExtendedCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable, Category=ExtendedActivatableWidget)
	void RegisterBinding(FDataTableRowHandle InputAction, const FInputActionExecutedDelegate& Callback,
		FInputActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category=ExtendedActivatableWidget)
	void UnregisterBinding(FInputActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category=ExtendedActivatableWidget)
	void UnregisterAllBindings();

private:
	TArray<FUIActionBindingHandle> BindingHandles;

	
};
