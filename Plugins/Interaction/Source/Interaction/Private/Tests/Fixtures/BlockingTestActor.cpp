// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockingTestActor.h"


// Sets default values
ABlockingTestActor::ABlockingTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Block);
	BoxComponent->SetBoxExtent(FVector(30, 30, 30));
	
}

// Called when the game starts or when spawned
void ABlockingTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlockingTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

