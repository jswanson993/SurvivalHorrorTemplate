#include "InteractableTest.h"
#include "Misc/AutomationTest.h"
#include "InteractionComponent.h"
#include "Tests/AutomationEditorCommon.h"

namespace InteractionTests
{
	/*
	UWorld* World;
	UInteractionComponent* InteractionComponent;
	AActor* Actor;

	void InteractSetup()
	{
		Actor = World->SpawnActor<AActor>(FVector::Zero(), FRotator::ZeroRotator);
		InteractionComponent = NewObject<UInteractionComponent>(Actor);
	}

	void InteractTearDown()
	{
		InteractionComponent->DestroyComponent();
		Actor->Destroy();
	}

	IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SimpleLine, "Interact.InteractComponent.SimpleLine",
									 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

	bool FInteractComponent_SimpleLine::RunTest(const FString& Parameters)
	{
		World = FAutomationEditorCommonUtils::CreateNewMap();
		{
			InteractSetup();
			InteractionComponent->SetTraceStyle(ETraceStyle::Single);
			InteractionComponent->SetTraceShape(ETraceShape::Line);
			FVector SpawnLocation = FVector(10, 0, 0);
			AInteractableTest* InteractableTest = World->SpawnActor<AInteractableTest>(SpawnLocation, FRotator::ZeroRotator);
			FActorComponentTickFunction* TickFunction = new FActorComponentTickFunction;
			TickFunction->Target = InteractionComponent;
			TickFunction->RegisterTickFunction(World->GetCurrentLevel());
			InteractionComponent->TickComponent(0, LEVELTICK_All, TickFunction);
			
			TArray<FHitResult> HitResults = InteractionComponent->GetHitResults();
			
			TestEqual(TEXT("Single trace should return one hit result when there is an actor in front of trace"),
				HitResults.Num(), 1);
			
			if(HitResults.Num() > 0)
			{
				UE_LOG(LogTemp,Warning,TEXT("Gets here"))
				TestEqual(TEXT("Hit result actor should be same as interactable test actor when actor in front of trace"),
					HitResults[0].GetActor()->GetName(), InteractableTest->GetName());;
			}
			InteractableTest->Destroy();
			InteractTearDown();
		}
	
		// Make the test pass by returning true, or fail by returning false.
		return true;
	}
	*/
}