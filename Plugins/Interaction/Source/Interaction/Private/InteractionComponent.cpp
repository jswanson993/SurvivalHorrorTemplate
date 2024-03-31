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

void UInteractionComponent::UpdateHitActors()
{
	TArray<AActor*> LastHitActors = HitActors;
	TArray<AActor*> CurrentHitActors;
	for (FHitResult Hit : GetHitResults())
	{
		if(Hit.bBlockingHit)
		{
			AActor* HitActor = Hit.GetActor();
			
			if(!HitActor->Implements<IInteractable>()){continue;}
			auto Interactable = Cast<IInteractable>(HitActor);
			if(Interactable == nullptr){ continue;}
			Interactable->Selected();
			
			CurrentHitActors.Add(HitActor);
			if(LastHitActors.Contains(HitActor))
			{
				LastHitActors.Remove(HitActor);
			}
		}
	}

	for(AActor* Actor : LastHitActors)
	{
		auto Interactable = Cast<IInteractable>(Actor);
		if(Interactable == nullptr){continue;}
		Interactable->Unselected();
	}

	HitActors = CurrentHitActors;
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PerformTrace();
	UpdateHitActors();
}

TArray<FHitResult> UInteractionComponent::GetHitResults()
{
	switch (TraceStyle)
	{
	case Single:
		HitResults.Empty();
		HitResults.Add(HitResult);
		break;
	default:
		break;
	}
	return HitResults;
}

void UInteractionComponent::PerformTrace()
{
	switch(TraceStyle)
	{
	case Single:
		TraceSingle();
		break;
	case Multi:
		TraceMulti();
		break;
	default:
		break;
	}
}

void UInteractionComponent::TraceSingle()
{
	UWorld* World = GetWorld();
	if(World == nullptr){ return; }
	FVector StartPoint = GetComponentLocation();
	FVector EndPoint = (GetForwardVector() * TraceDistance) + GetComponentLocation();
	FRotator Orientation = GetComponentRotation();
	switch (TraceShape)
	{
	case ECollisionShape::Line:

		UKismetSystemLibrary::LineTraceSingle(World,GetComponentLocation(), EndPoint, TraceTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ECollisionShape::Box:
		UKismetSystemLibrary::BoxTraceSingle(World, StartPoint, EndPoint, BoxHalfSize, Orientation, TraceTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ECollisionShape::Capsule:
		UKismetSystemLibrary::CapsuleTraceSingle(World, StartPoint, EndPoint, CapsuleRadius, CapsuleHalfHeight, TraceTypeQuery,
			false,ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ECollisionShape::Sphere:
		UKismetSystemLibrary::SphereTraceSingle(World, StartPoint, EndPoint, SphereRadius, TraceTypeQuery, false, ActorsToIgnore,
			DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	default:
		break;
	}
}

void UInteractionComponent::TraceMulti()
{
	const UWorld* World = GetWorld();
	if(World == nullptr){ return; }
	const FVector StartPoint = GetComponentLocation();
	const  FVector EndPoint = (GetForwardVector() * TraceDistance) + GetComponentLocation();
	const FRotator Orientation = GetComponentRotation();
	switch (TraceShape)
	{
	case ECollisionShape::Line:

		UKismetSystemLibrary::LineTraceMulti(World,GetComponentLocation(), EndPoint, TraceTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ECollisionShape::Box:
		UKismetSystemLibrary::BoxTraceMulti(World, StartPoint, EndPoint, BoxHalfSize, Orientation, TraceTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ECollisionShape::Capsule:
		UKismetSystemLibrary::CapsuleTraceMulti(World, StartPoint, EndPoint, CapsuleRadius, CapsuleHalfHeight, TraceTypeQuery,
			false,ActorsToIgnore, DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ECollisionShape::Sphere:
		UKismetSystemLibrary::SphereTraceMulti(World, StartPoint, EndPoint, SphereRadius, TraceTypeQuery, false, ActorsToIgnore,
			DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	default:
		break;
	}
}

