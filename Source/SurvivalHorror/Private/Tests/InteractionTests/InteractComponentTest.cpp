/*
#include "InteractableTest.h"
#include "Misc/AutomationTest.h"
#include "InteractionComponent.h"
#include "InteractionTestActor.h"
#include "Tests/AutomationEditorCommon.h"

namespace InteractionTests
{
	UWorld* World;
	UInteractionComponent* InteractionComponent;
	AInteractionTestActor* Actor;
	AInteractableTest* InteractableTest;

	DEFINE_LATENT_AUTOMATION_COMMAND(FSetup);
	bool FSetup::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			Actor = World->SpawnActor<AInteractionTestActor>(FVector::Zero(), FRotator::ZeroRotator);
			InteractionComponent = Actor->GetInteractionComponent();
			Actor->PreInitializeComponents();
			Actor->InitializeComponents();
			Actor->PostInitializeComponents();
			//World->BeginPlay();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND(FTearDown);
	bool FTearDown::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			InteractionComponent->DestroyComponent();
			Actor->Destroy();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCheckHitResults, FAutomationTestBase*, Test);
	bool FCheckHitResults::Update()
	{
		if(InteractionComponent == nullptr)
		{
			return false;
		}
		const TArray<FHitResult> HitResults = InteractionComponent->GetHitResults();
		if(HitResults.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("World is ticking: %hhd"), World->bInTick);
			//World->Tick(ELevelTick::LEVELTICK_All, 0);
			//Actor->Tick(0);
			return false;
		}

		Test->TestEqual(TEXT("Single trace should return one hit result when there is an actor in front of trace"),
	HitResults.Num(), 1);

		return true;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND(FSpawnInteractable);
	bool FSpawnInteractable::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			FVector SpawnLocation = InteractionComponent->GetComponentLocation() + (InteractionComponent->GetForwardVector() * 30);
			InteractableTest = World->SpawnActor<AInteractableTest>(SpawnLocation, FRotator::ZeroRotator);
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND(FDestroyInteractable);
	bool FDestroyInteractable::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			InteractableTest->Destroy();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND(FSetWorld);
	bool FSetWorld::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			World = GEditor->GetPIEWorldContext()->World();
			return true;
		}
		return false;
	}

	DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FSetInteractionDefaults, ETraceStyle, TraceStyle, ETraceShape, TraceShape);
	bool FSetInteractionDefaults::Update()
	{
		if(GEditor->IsPlayingSessionInEditor())
		{
			InteractionComponent->SetTraceStyle(TraceStyle);
			InteractionComponent->SetTraceShape(TraceShape);
			return true;
		}
		return false;
	}

	IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SimpleLine, "Interact.InteractComponent.SimpleLine",
									 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

	bool FInteractComponent_SimpleLine::RunTest(const FString& Parameters)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(FString("/Game/Tests/TestMaps/VoidWorld")));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetWorld);
		{
			ADD_LATENT_AUTOMATION_COMMAND(FSetup);
			ADD_LATENT_AUTOMATION_COMMAND(FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Line));
			ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable);
			ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this));
			ADD_LATENT_AUTOMATION_COMMAND(FTearDown);
			ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
		}
	
		// Make the test pass by returning true, or fail by returning false.
		return true;
	}
}
*/