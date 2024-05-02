// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BlockingTestActor.generated.h"

UCLASS()
class INTERACTION_API ABlockingTestActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UBoxComponent* BoxComponent;

public:
	// Sets default values for this actor's properties
	ABlockingTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
