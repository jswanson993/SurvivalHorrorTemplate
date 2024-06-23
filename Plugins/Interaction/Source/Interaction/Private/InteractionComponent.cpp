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



// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformTrace();
}

TArray<AActor*> UInteractionComponent::GetInteractables()
{
	TArray<AActor*> Interactables; 
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
	if(HitResults.Num() > 0)
	{
		for(FHitResult Result : HitResults)
		{
			if(Result.GetActor() != nullptr && Result.GetActor()->Implements<UInteractable>())
			{
				Interactables.Add(Result.GetActor());
			}
		}
	}
	return Interactables;
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
	const FName TraceTag("InteractionTrace");
	World->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = TraceTag;
	FCollisionResponseParams ResponseParams;
	
	switch (TraceShape)
	{
	case ETraceShape::Line:
		World->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, TraceChannel, QueryParams, ResponseParams);
		break;
	default:
		FCollisionShape CollisionShape = GetCollisionShapeFromTraceShape(TraceShape);
		World->SweepSingleByChannel(HitResult, StartPoint, EndPoint, Orientation.Quaternion(), TraceChannel, CollisionShape, QueryParams, ResponseParams);
		break;
	}
}

void UInteractionComponent::TraceMulti()
{
	UWorld* World = GetWorld();
	if(World == nullptr){ return; }
	const FVector StartPoint = GetComponentLocation();
	const  FVector EndPoint = (GetForwardVector() * TraceDistance) + GetComponentLocation();
	const FRotator Orientation = GetComponentRotation();
	const FName TraceTag("InteractionTrace");
	World->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = TraceTag;
	FCollisionResponseParams ResponseParams;
	
	switch (TraceShape)
	{
	case ETraceShape::Line:
		World->LineTraceMultiByChannel(HitResults, StartPoint, EndPoint, TraceChannel, QueryParams, ResponseParams);
		break;
	default:
		FCollisionShape CollisionShape = GetCollisionShapeFromTraceShape(TraceShape);
		World->SweepMultiByChannel(HitResults, StartPoint, EndPoint, Orientation.Quaternion(), TraceChannel, CollisionShape, QueryParams, ResponseParams);
		break;
	}
}

FCollisionShape UInteractionComponent::GetCollisionShapeFromTraceShape(ETraceShape Shape)
{
	const FCollisionShape CollisionShape;
	switch (Shape)
	{
		case ETraceShape::Box: return CollisionShape.MakeBox(BoxHalfSize);
		case ETraceShape::Sphere: return CollisionShape.MakeSphere(SphereRadius);
		case ETraceShape::Capsule: return CollisionShape.MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	default:
		return CollisionShape;
	}
}

