/*
#include "Editor.h"
#include "InteractableTest.h"
#include "Misc/AutomationTest.h"
#include "InteractionComponent.h"
#include "InteractionTestActor.h"
#include "Tests/AutomationEditorCommon.h"
#include "InteractionLatentCommands.h"
#include "Tests/AutomationCommon.h"

UWorld* World;
UInteractionComponent* InteractionComponent = nullptr;
AInteractionTestActor* Actor = nullptr;
AInteractableTest* InteractableTest = nullptr;
FString TestMapPath = TEXT("/Interaction/TestMaps/VoidWorld");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///				LINE SINGLE TESTS
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_LineSingle1,
                                 "Interact.InteractComponent.LineSingle.ShouldReturnOneHitResultWhenInteractableIsPlacedInFront",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_LineSingle1::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Line, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_LineSingle2,
                                 "Interact.InteractComponent.LineSingle.ShouldReturnNoHitResultsWhenInteractableIsPlacedToTheSide",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_LineSingle2::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Line, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 50, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_LineSingle3,
								 "Interact.InteractComponent.LineSingle.ShouldReturnNoHitResultsWhenInteractableIsPlacedBehind",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_LineSingle3::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Line, InteractionComponent));
		const FVector SpawnLocation = FVector(-30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_LineSingle4,
								 "Interact.InteractComponent.LineSingle.ShouldReturnOneHitResultWhenMultipleItemsAreInPath",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_LineSingle4::RunTest(const FString& Parameters)
{
	{
		AInteractableTest* InteractableTest2 = nullptr;
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Line, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		const FVector SpawnLocation2 = FVector(60, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest2, SpawnLocation2));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_LineSingle5,
								 "Interact.InteractComponent.LineSingle.ShouldNotReturnHitResultWhenInteractableIsMovedOutOfRange",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_LineSingle5::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Line, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		const FVector MoveToLocation = FVector(1000, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FMoveInteractable(InteractableTest, MoveToLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///				BOX SINGLE TESTS
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle1,
								 "Interact.InteractComponent.BoxSingle.ShouldReturnHitResultWhenInteractableIsPlacedInFront",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle1::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle2,
								 "Interact.InteractComponent.BoxSingle.ShouldReturnHitResultWhenInteractableIsPlacedBehind",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle2::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(-30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle3,
								 "Interact.InteractComponent.BoxSingle.ShouldReturnHitResultWhenInteractableIsPlacedAbove",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle3::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 0, 30);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle4,
								 "Interact.InteractComponent.BoxSingle.ShouldReturnHitResultWhenInteractableIsPlacedToTheSide",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle4::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 30, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle5,
								 "Interact.InteractComponent.BoxSingle.ShouldNotReturnHitResultWhenInteractableIsPlacedOutSideOfBox",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle5::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 100, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle6,
								 "Interact.InteractComponent.BoxSingle.ShouldNotReturnOneHitResultWhenMultipleInteractablesArePlaced",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle6::RunTest(const FString& Parameters)
{
	{
		AInteractableTest* InteractableTest2 = nullptr;
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		const FVector SpawnLocation2 = FVector(0, 30, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest2, SpawnLocation2));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_BoxSingle7,
								 "Interact.InteractComponent.BoxSingle.ShouldNotReturnHitResultWhenInteractableIsMovedOutOfRange",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_BoxSingle7::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		const FVector MoveToLocation = FVector(1000, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FMoveInteractable(InteractableTest, MoveToLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///				SPHERE SINGLE TESTS
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle1,
								 "Interact.InteractComponent.SphereSingle.ShouldReturnHitResultWhenInteractableIsPlacedInFront",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle1::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(15, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle2,
								 "Interact.InteractComponent.SphereSingle.ShouldReturnHitResultWhenInteractableIsPlacedBehind",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle2::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(-15, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle3,
								 "Interact.InteractComponent.SphereSingle.ShouldReturnHitResultWhenInteractableIsPlacedAbove",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle3::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 0, 15);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle4,
								 "Interact.InteractComponent.SphereSingle.ShouldReturnHitResultWhenInteractableIsPlacedToTheSide",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle4::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 15, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle5,
								 "Interact.InteractComponent.SphereSingle.ShouldNotReturnHitResultWhenInteractableIsPlacedOutSideOfSphere",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle5::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 100, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle6,
								 "Interact.InteractComponent.SphereSingle.ShouldReturnOneHitResultWhenMultipleInteractablesArePlaced",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle6::RunTest(const FString& Parameters)
{
	{
		AInteractableTest* InteractableTest2 = nullptr;
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		const FVector SpawnLocation2 = FVector(0, 30, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest2, SpawnLocation2));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_SphereSingle7,
								 "Interact.InteractComponent.SphereSingle.ShouldNotReturnHitResultWhenInteractableIsMovedOutOfRange",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_SphereSingle7::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Sphere, InteractionComponent));
		const FVector SpawnLocation = FVector(15, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		const FVector MoveToLocation = FVector(1000, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FMoveInteractable(InteractableTest, MoveToLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///				CAPSULE SINGLE TESTS
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle1,
								 "Interact.InteractComponent.CapsuleSingle.ShouldReturnHitResultWhenInteractableIsPlacedInFront",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle1::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Capsule, InteractionComponent));
		const FVector SpawnLocation = FVector(15, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle2,
								 "Interact.InteractComponent.CapsuleSingle.ShouldReturnHitResultWhenInteractableIsPlacedBehind",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle2::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Capsule, InteractionComponent));
		const FVector SpawnLocation = FVector(-15, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle3,
								 "Interact.InteractComponent.CapsuleSingle.ShouldReturnHitResultWhenInteractableIsPlacedAbove",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle3::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Capsule, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 0, 15);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle4,
								 "Interact.InteractComponent.CapsuleSingle.ShouldReturnHitResultWhenInteractableIsPlacedToTheSide",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle4::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Capsule, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 15, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle5,
								 "Interact.InteractComponent.CapsuleSingle.ShouldNotReturnHitResultWhenInteractableIsPlacedOutSideOfBox",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle5::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Capsule, InteractionComponent));
		const FVector SpawnLocation = FVector(0, 100, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle6,
								 "Interact.InteractComponent.CapsuleSingle.ShouldReturnOneHitResultWhenMultipleInteractablesArePlaced",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle6::RunTest(const FString& Parameters)
{
	{
		AInteractableTest* InteractableTest2 = nullptr;
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Capsule, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		const FVector SpawnLocation2 = FVector(0, 30, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest2, SpawnLocation2));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_CapsuleSingle7,
								 "Interact.InteractComponent.CapsuleSingle.ShouldNotReturnHitResultWhenInteractableIsMovedOutOfRange",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FInteractComponent_CapsuleSingle7::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Single, ETraceShape::Box, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		const FVector MoveToLocation = FVector(1000, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FMoveInteractable(InteractableTest, MoveToLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 0));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInteractComponent_LineMulti,
								 "Interact.InteractComponent.LineMulti.ShouldReturnHitResultWhenInteractableIsPlacedInFront",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FInteractComponent_LineMulti::RunTest(const FString& Parameters)
{
	{
		ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(TestMapPath));
		ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true))
		ADD_LATENT_AUTOMATION_COMMAND(FSetup(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(
			FSetInteractionDefaults(ETraceStyle::Multi, ETraceShape::Line, InteractionComponent));
		const FVector SpawnLocation = FVector(30, 0, 0);
		ADD_LATENT_AUTOMATION_COMMAND(FSpawnInteractable(InteractionComponent, InteractableTest, SpawnLocation));
		ADD_LATENT_AUTOMATION_COMMAND(FWaitForNextEngineFrameCommand());
		ADD_LATENT_AUTOMATION_COMMAND(FCheckHitResults(this, InteractionComponent, 1));
		ADD_LATENT_AUTOMATION_COMMAND(FTearDown(Actor, InteractionComponent));
		ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	}
	return true;
}
*/