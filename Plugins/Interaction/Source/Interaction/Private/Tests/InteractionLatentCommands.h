#pragma once
#include "InteractionTestActor.h"



	DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FSetup, AInteractionTestActor*&, OutActor, UInteractionComponent*&, OutInteractionComponent);
	inline bool FSetup::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			UWorld* World = GEditor->GetPIEWorldContext()->World();
			OutActor = World->SpawnActor<AInteractionTestActor>(FVector::Zero(), FRotator::ZeroRotator);
			OutInteractionComponent = OutActor->GetInteractionComponent();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FTearDown, AInteractionTestActor*&, Actor, UInteractionComponent*&, InteractionComponent);
	inline bool FTearDown::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			InteractionComponent->DestroyComponent();
			Actor->Destroy();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FCheckHitResults, FAutomationTestBase*, Test, UInteractionComponent*&, InteractionComponent, int, Expected);
	inline bool FCheckHitResults::Update()
	{
		if(InteractionComponent == nullptr)
		{
			return false;
		}
		
		const TArray<FHitResult> HitResults = InteractionComponent->GetHitResults();

		Test->TestEqual(TEXT("HitResults.Num() == Expected"),
	HitResults.Num(), Expected);

		return true;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FSpawnInteractable, UInteractionComponent*&, InteractionComponent, AInteractableTest*&, OutInteractableTest, FVector, SpawnLocation);
	inline bool FSpawnInteractable::Update()
	{
		UWorld* World = GEditor->GetPIEWorldContext()->World();
		if(GEditor->IsPlayingSessionInEditor())
		{
			OutInteractableTest = World->SpawnActor<AInteractableTest>(SpawnLocation, FRotator::ZeroRotator);
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FDestroyInteractable, AInteractableTest*&, InteractableTest);
	inline bool FDestroyInteractable::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			InteractableTest->Destroy();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FSetInteractionDefaults, ETraceStyle, TraceStyle, ETraceShape, TraceShape, UInteractionComponent*&, InteractionComponent);
	inline bool FSetInteractionDefaults::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			InteractionComponent->SetTraceStyle(TraceStyle);
			InteractionComponent->SetTraceShape(TraceShape);
			switch(TraceShape)
			{
			case ETraceShape::Line:
				InteractionComponent->SetTraceDistance(100);
				break;
			case ETraceShape::Box:
				InteractionComponent->SetBoxHalfSize(FVector(30,30,30));
				InteractionComponent->SetTraceDistance(0);
				break;
			case ETraceShape::Sphere:
				InteractionComponent->SetSphereRadius(30);
				InteractionComponent->SetTraceDistance(1);
				break;
			case ETraceShape::Capsule:
				InteractionComponent->SetCapsuleRadius(30);
				InteractionComponent->SetCapsuleHalfHeight(60);
				InteractionComponent->SetTraceDistance(1);
			default:
				break;
			}
			return true;
		}
		return false;
	}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FMoveInteractable, AInteractableTest*&, InteractableTest, FVector, MoveToLocation);
inline bool FMoveInteractable::Update()
{
	if(GEditor->IsPlayingSessionInEditor())
	{
		InteractableTest->SetActorLocation(MoveToLocation);
		return true;
	}
	return false;
}


