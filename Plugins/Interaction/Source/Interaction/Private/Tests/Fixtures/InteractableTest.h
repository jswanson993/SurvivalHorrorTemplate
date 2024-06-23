// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableTest.generated.h"

class UBoxComponent;

UCLASS()
class INTERACTION_API AInteractableTest : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY()
	UBoxComponent* BoxCollision;
	bool bIsSelected;
	bool bIsUnselected;
	bool bIsUsed;
	bool bIsAlternateUsed;
public:
	// Sets default values for this actor's properties
	AInteractableTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Selected_Implementation(AController* SelectedBy) override {bIsSelected = true;};
	virtual void Unselected_Implementation(AController* UnselectedBy) override {bIsUnselected = true;};
	virtual void Use_Implementation(AController* UsedBy) override { bIsUsed = true; };
	virtual void AlternateUse_Implementation(AController* AltUsedBy) override { bIsAlternateUsed = true; };

	const bool GetSelected() { return bIsSelected; };
	const bool GetUnselected() { return bIsUnselected; }
	const bool GetUse() { return bIsUsed; }
	const bool GetAlternativeUse() { return bIsAlternateUsed; }
	
};