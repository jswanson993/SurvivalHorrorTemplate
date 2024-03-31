// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableTest.generated.h"

class UBoxComponent;

UCLASS()
class SURVIVALHORROR_API AInteractableTest : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY()
	UBoxComponent* BoxCollision;
	bool bIsSelected;
	bool bIsUnselected;
public:
	// Sets default values for this actor's properties
	AInteractableTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Selected_Implementation() override {bIsSelected = true;};
	virtual void Unselected_Implementation() override {bIsUnselected = true;};
	virtual void Use_Implementation() override {};
	virtual void AlternativeUse_Implementation() override {};

	const bool GetSelected() { return bIsSelected; };
	const bool GetUnselected() { return bIsUnselected; }
};
