// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Interactable.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("InteractArrow");
	ArrowComponent->SetupAttachment(this);
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	ActorsToIgnore.Add(this->GetOwner());
}

void UInteractionComponent::UpdateHitActors()
{
	TArray<AActor*> LastHitActors = HitActors;
	TArray<AActor*> CurrentHitActors;
	TArray<FHitResult> NewHitResults = GetHitResults();
	bIsUpdatingActors = true;
	//NewHitResults = TArray<FHitResult>::CopyToEmpty(GetHitResults(), GetHitResults().Num(), );
	for (FHitResult Hit : NewHitResults)
	{
		if(Hit.bBlockingHit)
		{
			AActor* HitActor = Hit.GetActor();
			
			
			if(HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				IInteractable::Execute_Selected(HitActor);
				CurrentHitActors.Add(HitActor);
			}

			if(LastHitActors.Contains(HitActor))
			{
				LastHitActors.Remove(HitActor);
			}
		}
	}

	for(AActor* Actor : LastHitActors)
	{
		if(Actor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Selected(Actor);
		}
	}

	HitActors = CurrentHitActors;
	bIsUpdatingActors = false;
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!bIsUpdatingActors)
	{
		PerformTrace();
		UpdateHitActors();
	}
}

TArray<FHitResult> UInteractionComponent::GetHitResults()
{
	switch (TraceStyle)
	{
	case Single:
		HitResults.Empty();
		if(HitResult.bBlockingHit)
		{
			HitResults.Add(HitResult);
		}
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
	case ETraceShape::Line:
		UKismetSystemLibrary::LineTraceSingleForObjects(World,GetComponentLocation(), EndPoint, ObjectTypeQuery,
		false, ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ETraceShape::Box:
		UKismetSystemLibrary::BoxTraceSingleForObjects(World, StartPoint, EndPoint, BoxHalfSize, Orientation, ObjectTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ETraceShape::Capsule:
		UKismetSystemLibrary::CapsuleTraceSingleForObjects(World, StartPoint, EndPoint, CapsuleRadius, CapsuleHalfHeight, ObjectTypeQuery,
			false,ActorsToIgnore, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ETraceShape::Sphere:
		UKismetSystemLibrary::SphereTraceSingleForObjects(World, StartPoint, EndPoint, SphereRadius, ObjectTypeQuery, false, ActorsToIgnore,
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
	case ETraceShape::Line:

		UKismetSystemLibrary::LineTraceMultiForObjects(World,GetComponentLocation(), EndPoint, ObjectTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ETraceShape::Box:
		UKismetSystemLibrary::BoxTraceMultiForObjects(World, StartPoint, EndPoint, BoxHalfSize, Orientation, ObjectTypeQuery,
			false, ActorsToIgnore, DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ETraceShape::Capsule:
		UKismetSystemLibrary::CapsuleTraceMultiForObjects(World, StartPoint, EndPoint, CapsuleRadius, CapsuleHalfHeight, ObjectTypeQuery,
			false,ActorsToIgnore, DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	case ETraceShape::Sphere:
		UKismetSystemLibrary::SphereTraceMultiForObjects(World, StartPoint, EndPoint, SphereRadius, ObjectTypeQuery, false, ActorsToIgnore,
			DrawDebugType, HitResults, true, TraceColor, TraceHitColor, DrawTime);
		break;
	default:
		break;
	}
}

