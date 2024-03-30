// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Interactable.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ActorsToIgnore.Add(this->GetOwner());
	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector EndPoint = (GetForwardVector() * TraceDistance) + GetComponentLocation();
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),GetComponentLocation(), EndPoint, TraceTypeQuery1,
		false, ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
	if(HitResult.bBlockingHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor->Implements<IInteractable>())
		{
			
		}
	}
}

