// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionTestActor.h"

#include "InteractionComponent.h"


// Sets default values
AInteractionTestActor::AInteractionTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);
	InteractionComponent->SetObjectQuery(ObjectTypes);
	
	
}

// Called when the game starts or when spawned
void AInteractionTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

