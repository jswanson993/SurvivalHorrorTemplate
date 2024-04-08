// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionTestActor.generated.h"

class UInteractionComponent;

UCLASS()
class SURVIVALHORROR_API AInteractionTestActor : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
	UInteractionComponent* InteractionComponent;

public:
	// Sets default values for this actor's properties
	AInteractionTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UInteractionComponent* GetInteractionComponent() { return InteractionComponent; }
};
