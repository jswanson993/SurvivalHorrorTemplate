// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableTest.h"

#include "Components/BoxComponent.h"


// Sets default values
AInteractableTest::AInteractableTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollision->InitBoxExtent(FVector(10,10,10));
	RootComponent = BoxCollision;
}

// Called when the game starts or when spawned
void AInteractableTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

