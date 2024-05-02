#include "../../../../../../Source/SurvivalHorror/Private/Tests/WorldFixture.h"
#include "Fixtures/BlockingTestActor.h"
#if WITH_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "InteractableTest.h"
#include "InteractionComponent.h"
#include "Fixtures/InteractionTestActor.h"
#include "Tests/AutomationEditorCommon.h"
#include "Tests/AutomationCommon.h"


BEGIN_DEFINE_SPEC(FInteractComponent, "Interact.InteractComponent",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter);

	TUniquePtr<FWorldFixture> WorldFixture;
	UWorld* World;
	AInteractableTest* Interactable;
	AInteractableTest* Interactable2;
	ABlockingTestActor* BlockingTestActor;
	AInteractionTestActor* Actor;
	UInteractionComponent* InteractionComponent;
	bool bMapLoaded = false;

	int NumActualHitResults = 0;
	int NumExpectedHitResults = 0;
	TArray<FHitResult> ActualHitResults;
	FTimespan Timeout = FTimespan(10);

	void GetHitResults()
	{
		NumActualHitResults = 0;
		while (NumActualHitResults == 0)
		{
			ActualHitResults = InteractionComponent->GetHitResults();
			NumActualHitResults = ActualHitResults.Num();
		}
	}

	void SingleShapeTests()
	{
		Describe("No Actor In Trace", [this]()
		{
			BeforeEach([this]()
			{
				Interactable->SetActorLocation(FVector(10000, 10000, 10000));
				NumExpectedHitResults = 0;
			});
			LatentIt("Should not have any hits", EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::AnyThread, [this, Done]()
				{
					bool bNextFrameWaited = false;
					while (bNextFrameWaited == false)
					{
						bNextFrameWaited = FWaitForNextEngineFrameCommand().Update();
					}

					AsyncTask(ENamedThreads::GameThread, [this, Done]()
					{
						NumActualHitResults = InteractionComponent->GetHitResults().Num();
						TestEqual(FString::Printf(
							          TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"),
							          NumActualHitResults, NumExpectedHitResults), NumActualHitResults,
						          NumExpectedHitResults);
						Done.Execute();
					});
				});
			});
		});
		BeforeEach([this]()
		{
			NumExpectedHitResults = 1;
		});
		Describe("Interactable Inside Trace", [this]()
		{
			Describe("Interactable In Front of Actor", [this]()
			{
				LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
				{
					AsyncTask(ENamedThreads::AnyThread, [this, Done]()
					{
						AsyncTask(ENamedThreads::GameThread, [this]()
						{
							Interactable->SetActorLocation(FVector(30, 0, 0));
						});
						GetHitResults();

						Done.Execute();
					});
				});
				It("Should Find One Hit Result", [this]()
				{
					TestEqual(FString::Printf(TEXT("NumExpectedHitResults: %d == NumActualHitResults: %d"),
					                          NumExpectedHitResults, NumActualHitResults),
					          NumExpectedHitResults, NumActualHitResults);
				});
				It("Should Find Interactable", [this]()
				{
					if (ActualHitResults.Num() == 0)
					{
						AddError("Test Returned 0 Hit Results");
					}
					if (ActualHitResults.Num() > NumExpectedHitResults)
					{
						AddError("Test Returned More Hit Results Than Expected");
					}
					AActor* ActualActor = ActualHitResults[0].GetActor();
					AActor* ExpectedActor = Cast<AActor>(Interactable);
					TestEqual(FString::Printf(TEXT("ExpectedActor: %s == ActualActor: %s"),
					                          *ActualActor->GetName(), *ExpectedActor->GetName()),
					          ActualActor, ExpectedActor);
				});
			});
			Describe("Interactable Next To Actor", [this]()
			{
				LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
				{
					AsyncTask(ENamedThreads::AnyThread, [this, Done]()
					{
						AsyncTask(ENamedThreads::GameThread, [this]()
						{
							Interactable->SetActorLocation(FVector(0, 30, 0));
						});
						GetHitResults();

						Done.Execute();
					});
				});
				It("Should Find One Hit Result", [this]()
				{
					TestEqual(FString::Printf(TEXT("NumExpectedHitResults: %d == NumActualHitResults: %d"),
					                          NumExpectedHitResults, NumActualHitResults),
					          NumExpectedHitResults, NumActualHitResults);
				});
				It("Should Find Interactable", [this]()
				{
					if (ActualHitResults.Num() == 0)
					{
						AddError("Test Returned 0 Hit Results");
					}
					if (ActualHitResults.Num() > NumExpectedHitResults)
					{
						AddError("Test Returned More Hit Results Than Expected");
					}
					AActor* ActualActor = ActualHitResults[0].GetActor();
					AActor* ExpectedActor = Cast<AActor>(Interactable);
					TestEqual(FString::Printf(TEXT("ExpectedActor: %s == ActualActor: %s"),
					                          *ActualActor->GetName(), *ExpectedActor->GetName()),
					          ActualActor, ExpectedActor);
				});
			});
			Describe("Interactable Behind Actor", [this]()
			{
				LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
				{
					AsyncTask(ENamedThreads::AnyThread, [this, Done]()
					{
						AsyncTask(ENamedThreads::GameThread, [this]()
						{
							Interactable->SetActorLocation(FVector(-30, 0, 0));
						});
						GetHitResults();

						Done.Execute();
					});
				});
				It("Should Find One Hit Result", [this]()
				{
					TestEqual(FString::Printf(TEXT("NumExpectedHitResults: %d == NumActualHitResults: %d"),
					                          NumExpectedHitResults, NumActualHitResults),
					          NumExpectedHitResults, NumActualHitResults);
				});
				It("Should Find Interactable", [this]()
				{
					if (ActualHitResults.Num() == 0)
					{
						AddError("Test Returned 0 Hit Results");
					}
					if (ActualHitResults.Num() > NumExpectedHitResults)
					{
						AddError("Test Returned More Hit Results Than Expected");
					}
					AActor* ActualActor = ActualHitResults[0].GetActor();
					AActor* ExpectedActor = Cast<AActor>(Interactable);
					TestEqual(FString::Printf(TEXT("ExpectedActor: %s == ActualActor: %s"),
					                          *ActualActor->GetName(), *ExpectedActor->GetName()),
					          ActualActor, ExpectedActor);
				});
			});

			Describe("Interactable Above Actor", [this]()
			{
				LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
				{
					AsyncTask(ENamedThreads::AnyThread, [this, Done]()
					{
						AsyncTask(ENamedThreads::GameThread, [this]()
						{
							Interactable->SetActorLocation(FVector(0, 0, 30));
						});
						GetHitResults();

						Done.Execute();
					});
				});
				It("Should Find One Hit Result", [this]()
				{
					TestEqual(FString::Printf(TEXT("NumExpectedHitResults: %d == NumActualHitResults: %d"),
					                          NumExpectedHitResults, NumActualHitResults),
					          NumExpectedHitResults, NumActualHitResults);
				});
				It("Should Find Interactable", [this]()
				{
					if (ActualHitResults.Num() == 0)
					{
						AddError("Test Returned 0 Hit Results");
					}
					if (ActualHitResults.Num() > NumExpectedHitResults)
					{
						AddError("Test Returned More Hit Results Than Expected");
					}
					AActor* ActualActor = ActualHitResults[0].GetActor();
					AActor* ExpectedActor = Cast<AActor>(Interactable);
					TestEqual(FString::Printf(TEXT("ExpectedActor: %s == ActualActor: %s"),
					                          *ActualActor->GetName(), *ExpectedActor->GetName()),
					          ActualActor, ExpectedActor);
				});
			});
		});
		Describe("Multiple Actors Inside Trace", [this]()
		{
			BeforeEach([this]()
			{
				Interactable2 = World->SpawnActor<AInteractableTest>();
				Interactable->SetActorLocation(FVector(30, 0, 0));
				Interactable2->SetActorLocation(FVector(0, 30, 0));
			});

			LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::AnyThread, [this, Done]()
				{
					GetHitResults();
					Done.Execute();
				});
			});
			It("Should Find One Hit Result", [this]()
			{
				TestEqual(FString::Printf(TEXT("NumExpectedHitResults: %d == NumActualHitResults: %d"),
				                          NumExpectedHitResults, NumActualHitResults), NumExpectedHitResults,
				          NumActualHitResults);
			});
			LatentAfterEach(EAsyncExecution::TaskGraph, [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::GameThread, [this, Done]()
				{
					Interactable2->Destroy();
					Done.Execute();
				});
			});
		});
	}

	void MultiShapeTests()
	{
		BeforeEach([this]()
		{
			Interactable2 = World->SpawnActor<AInteractableTest>();
		});
		Describe("Interactables Not In Trace", [this]()
		{
			BeforeEach([this]()
			{
				Interactable->SetActorLocation(FVector(0, 1000, 0));
				Interactable2->SetActorLocation(FVector(0, -1000, 0));
				NumExpectedHitResults = 0;
			});
			LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::AnyThread, [this, Done]()
				{
					bool bNextFrameReady = false;
					while (bNextFrameReady == false)
					{
						bNextFrameReady = FWaitForNextEngineFrameCommand().Update();
					}
					Done.Execute();
				});
			});
			It("Should Not Find Any Hit Result", [this]()
			{
				NumActualHitResults = InteractionComponent->GetHitResults().Num();
				TestEqual(FString::Printf(TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"),
				                          NumActualHitResults, NumExpectedHitResults), NumActualHitResults,
				          NumExpectedHitResults);
			});
		});
		Describe("Interactables In Trace", [this]()
		{
			BeforeEach([this]()
			{
				NumExpectedHitResults = 2;
				Interactable->SetActorLocation(FVector(0, 30, 0));
				Interactable2->SetActorLocation(FVector(0, -30, 0));
			});
			LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::AnyThread, [this, Done]()
				{
					GetHitResults();
					Done.Execute();
				});
				It("Should Find Two Hit Results", [this]()
				{
					TestEqual(FString::Printf(TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"),
					                          NumActualHitResults, NumExpectedHitResults), NumActualHitResults,
					          NumExpectedHitResults);
				});
				It("Should Find Interactables From Results", [this]()
				{
					AActor* Interactable1Actor = Cast<AActor>(Interactable);
					AActor* Interactable2Actor = Cast<AActor>(Interactable2);
					TArray<AActor*> HitActors;
					for(FHitResult HitResult : ActualHitResults)
					{
						HitActors.Add(HitResult.GetActor());
					}
					TestTrue("HitActors Contains Interactable", HitActors.Contains(Interactable1Actor));
					TestTrue("HitActors Contains Interactable2", HitActors.Contains(Interactable2Actor));
				});
			});
		});
		Describe("Interactable Blocked By Actor", [this]()
		{
			BeforeEach([this]
			{
				NumExpectedHitResults = 1;
				BlockingTestActor = World->SpawnActor<ABlockingTestActor>();
				Interactable->SetActorLocation(FVector(70,0,0));
				Interactable2->SetActorLocation(FVector(10000, 0,0));
				BlockingTestActor->SetActorLocation(FVector(30, 0, 0));
			});

			LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done){
				AsyncTask(ENamedThreads::AnyThread, [this, Done]()
				{
					GetHitResults();
					Done.Execute();
				});
			});

			It("Should Find One Hit Result Behind Blocking Actor", [this]()
			{
				TestEqual(FString::Printf(TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"),
					NumActualHitResults, NumExpectedHitResults), NumActualHitResults, NumExpectedHitResults);
			});

			It("Should Find Interactable", [this]()
			{
				AActor* InteractableActor = Cast<AActor>(Interactable);
				if(ActualHitResults.Num() == 0)
				{
					AddError("Hit Result Count Should Not Be Zero");
				}
				AActor* HitActor = ActualHitResults[0].GetActor();
				TestEqual(FString::Printf(TEXT("HitActor: %s == Interactable: %s"), *HitActor->GetName(), *InteractableActor->GetName()), HitActor, InteractableActor);
			});

			AfterEach([this]()
			{
				BlockingTestActor->Destroy();
			});
		});

		AfterEach([this]()
		{
			Interactable2->Destroy();
		});
	}

END_DEFINE_SPEC(FInteractComponent);

void FInteractComponent::Define()
{
	Describe("Trace", [this]()
	{
		LatentBeforeEach(EAsyncExecution::TaskGraph, [this](const FDoneDelegate& Done)
		{
			AsyncTask(ENamedThreads::GameThread, [this, Done]()
			{
				const FString TestWorldName = TEXT("/Interaction/TestMaps/VoidWorld");
				if (bMapLoaded == false)
				{
					FAutomationEditorCommonUtils::LoadMap(TestWorldName);
					bMapLoaded = true;
				}
				FStartPIECommand(true).Update();
				Done.Execute();
			});
		});
		LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
		{
			AsyncTask(ENamedThreads::AnyThread, [this, Done]()
			{
				bool bWorldReady;
				do
				{
					bWorldReady = GEditor->IsPlayingSessionInEditor();
				}
				while (bWorldReady == false);

				AsyncTask(ENamedThreads::GameThread, [this, Done]()
				{
					World = GEditor->GetPIEWorldContext()->World();
					if (World != nullptr)
					{
						World = GEditor->GetPIEWorldContext()->World();
						Interactable = World->SpawnActor<AInteractableTest>();
						Actor = World->SpawnActor<AInteractionTestActor>();
						InteractionComponent = Actor->GetInteractionComponent();
						Actor->SetActorLocation(FVector(0, 0, 0));
						Done.Execute();
					}
				});
			});
		});
		Describe("Single", [this]()
		{
			Describe("Line", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(Line);
				});
				Describe("Interactable Is In Trace", [this]()
				{
					BeforeEach([this]()
					{
						Interactable->SetActorLocation(FVector(30, 0, 0));
						InteractionComponent->SetTraceDistance(100);
						NumExpectedHitResults = 1;
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							GetHitResults();
							Done.Execute();
						});
					});
					It("Should Have One Hit When One Interactable Is present", [this]()
					{
						TestEqual(FString::Printf(
							          TEXT("ActualHitResults: %d == ExpectedHitResults: %d"), NumActualHitResults,
							          NumExpectedHitResults), NumActualHitResults, NumExpectedHitResults);
					});
					It("Should Return Interactable As HitResult", [this]()
					{
						AActor* ActualActor = Cast<AActor>(Interactable);
						AActor* HitActor = ActualHitResults[0].GetActor();
						TestEqual(FString::Printf(
							          TEXT("Interactable: %s == HitActor: %s"), *Interactable->GetName(),
							          *HitActor->GetName()), ActualActor, HitActor);
					});
					Describe("Multiple Interactables", [this]
					{
						BeforeEach([this]()
						{
							Interactable2 = World->SpawnActor<AInteractableTest>();
							Interactable2->SetActorLocation(FVector(60, 0, 0));
						});
						BeforeEach([this]()
						{
							Interactable2->SetActorLocation(FVector(60, 0, 0));
						});
						LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
						{
							AsyncTask(ENamedThreads::AnyThread, [this, Done]()
							{
								GetHitResults();
								Done.Execute();
							});
						});
						It("Should Have One Hit When Multiple Interactables Are Present", [this]()
						{
							TestEqual(FString::Printf(
								          TEXT("ActualHitResults: %d == ExpectedHitResults: %d"), NumActualHitResults,
								          NumExpectedHitResults), NumActualHitResults, NumExpectedHitResults);
						});
						It("Should Not Return Second Interactable As HitResult", [this]()
						{
							AActor* ActualActor = Cast<AActor>(Interactable);
							AActor* HitActor = ActualHitResults[0].GetActor();
							TestEqual(FString::Printf(
								          TEXT("Interactable: %s == HitActor: %s"), *Interactable->GetName(),
								          *HitActor->GetName()), ActualActor, HitActor);
						});
						LatentAfterEach(EAsyncExecution::TaskGraph, [this](const FDoneDelegate& Done)
						{
							AsyncTask(ENamedThreads::GameThread, [this, Done]()
							{
								Interactable2->Destroy();
								Done.Execute();
							});
						});
					});
				});
				Describe("Interactable Is Out Of Trace", [this]()
				{
					BeforeEach([this]()
					{
						Interactable->SetActorLocation(FVector(0, 30, 0));
						InteractionComponent->SetTraceDistance(100);
						NumExpectedHitResults = 0;
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							bool bNextFrameReady = false;
							while (bNextFrameReady == false)
							{
								bNextFrameReady = FWaitForNextEngineFrameCommand().Update();
							}
							Done.Execute();
						});
					});

					It("Should Not Have Any Result When Interactable Is Not In Path", [this]()
					{
						const int HitResultsNum = InteractionComponent->GetHitResults().Num();
						TestEqual(FString::Printf(
							          TEXT("HitResultsNum: %d == NumExpectedHitResults: %d"), HitResultsNum,
							          NumExpectedHitResults), HitResultsNum, NumExpectedHitResults);
					});
				});
				Describe("Non Channel Actor In Trace", [this]()
				{
					BeforeEach([this]()
					{
						NumExpectedHitResults = 0;
						BlockingTestActor = World->SpawnActor<ABlockingTestActor>();
						BlockingTestActor->SetActorLocation(FVector(40, 0, 0));
						Interactable->SetActorLocation(FVector(90, 0, 0));
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							GetHitResults();
							Done.Execute();
						});
					});
					It("Should Find One Hit Result", [this]()
					{
						TestEqual(FString::Printf(
							          TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"), NumActualHitResults,
							          NumExpectedHitResults), NumActualHitResults, NumExpectedHitResults);
					});
					AfterEach([this]()
					{
						BlockingTestActor->Destroy();
					});
				});
				AfterEach([this]()
				{
					InteractionComponent->SetTraceDistance(0);
				});
			});
			Describe("Sphere", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Sphere);
					InteractionComponent->SetSphereRadius(60);
				});
				SingleShapeTests();
			});
			Describe("Box", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Box);
					InteractionComponent->SetBoxHalfSize(FVector(60, 60, 60));
				});
				SingleShapeTests();
			});
			Describe("Capsule", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Capsule);
					InteractionComponent->SetCapsuleRadius(60);
					InteractionComponent->SetCapsuleHalfHeight(90);
				});
				SingleShapeTests();
			});
		});
		Describe("Multi", [this]()
		{
			BeforeEach([this]()
			{
				Interactable2 = World->SpawnActor<AInteractableTest>();
			});
			Describe("Line", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Line);
					InteractionComponent->SetTraceDistance(1000);
				});
				Describe("Interactables Not In Trace", [this]()
				{
					BeforeEach([this]()
					{
						Interactable->SetActorLocation(FVector(0, 1000, 0));
						Interactable2->SetActorLocation(FVector(0, -1000, 0));
						NumExpectedHitResults = 0;
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							bool bNextFrameReady = false;
							while (bNextFrameReady == false)
							{
								bNextFrameReady = FWaitForNextEngineFrameCommand().Update();
							}
							Done.Execute();
						});
					});
					It("Should Not Find Any Hit Result", [this]()
					{
						NumActualHitResults = InteractionComponent->GetHitResults().Num();
						TestEqual(FString::Printf(TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"),
						                          NumActualHitResults, NumExpectedHitResults), NumActualHitResults,
						          NumExpectedHitResults);
					});
				});
				Describe("Interactables In Trace", [this]()
				{
					BeforeEach([this]()
					{
						NumExpectedHitResults = 2;
						Interactable->SetActorLocation(FVector(20, 0, 0));
						Interactable2->SetActorLocation(FVector(40, 0, 0));
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							GetHitResults();
							Done.Execute();
						});
					});
					It("Should Find Two Hit Results", [this]()
					{
						TestEqual(FString::Printf(TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"),
						                          NumActualHitResults, NumExpectedHitResults), NumActualHitResults,
						          NumExpectedHitResults);
					});
					It("Should Have Results That Match The Interactables", [this]()
					{
						const AActor* InteractableActor = Cast<AActor>(Interactable);
						const AActor* Interactable2Actor = Cast<AActor>(Interactable2);
						TestTrue(FString::Printf(TEXT("ActualHitResults Contains Interactable: %s"),
						                         *Interactable->GetName()), ActualHitResults.ContainsByPredicate(
							         [InteractableActor](const FHitResult& Result)
							         {
								         return Result.GetActor() == InteractableActor;
							         }));
						TestTrue(FString::Printf(TEXT("ActualHitResults Contains Interactable: %s"),
						                         *Interactable2->GetName()), ActualHitResults.ContainsByPredicate(
							         [Interactable2Actor](const FHitResult& Result)
							         {
								         return Result.GetActor() == Interactable2Actor;
							         }));
					});
				});
				Describe("One Interactable In Trace", [this]()
				{
					BeforeEach([this]()
					{
						NumExpectedHitResults = 1;
						Interactable->SetActorLocation(FVector(20, 0, 0));
						Interactable2->SetActorLocation(FVector(0, 1000, 0));
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, Timeout, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							GetHitResults();
							Done.Execute();
						});
					});
					It("Should Find One Hit Result", [this]()
					{
						TestEqual(FString::Printf(TEXT("NumActualHitResults: %d == NumExpectedHitResults %d"),
						                          NumActualHitResults, NumExpectedHitResults), NumActualHitResults,
						          NumExpectedHitResults);
					});
					It("Should Find The Correct Interactable", [this]()
					{
						AActor* InteractableActor = Cast<AActor>(Interactable);
						if (ActualHitResults.Num() == 0)
						{
							AddError("Actual Hit Results Should Not Be Zero");
						}
						AActor* HitActor = ActualHitResults[0].GetActor();
						TestEqual(FString::Printf(
							          TEXT("ActualHitResult: %s == Interactable: %s"), *HitActor->GetName(),
							          *Interactable->GetName()), HitActor, InteractableActor);
					});
				});
				Describe("Non Channel Actor In Trace", [this]()
				{
					BeforeEach([this]()
					{
						NumExpectedHitResults = 1;
						BlockingTestActor = World->SpawnActor<ABlockingTestActor>();
						BlockingTestActor->SetActorLocation(FVector(40, 0, 0));
						Interactable->SetActorLocation(FVector(90, 0, 0));
					});
					LatentBeforeEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
					{
						AsyncTask(ENamedThreads::AnyThread, [this, Done]()
						{
							GetHitResults();
							Done.Execute();
						});
					});
					It("Should Find One Hit Result", [this]()
					{
						TestEqual(FString::Printf(
							          TEXT("NumActualHitResults: %d == NumExpectedHitResults: %d"), NumActualHitResults,
							          NumExpectedHitResults), NumActualHitResults, NumExpectedHitResults);
					});
					AfterEach([this]()
					{
						BlockingTestActor->Destroy();
					});
				});
				AfterEach([this]()
				{
					InteractionComponent->SetTraceDistance(0);
				});
			});
			Describe("Sphere", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Sphere);
					InteractionComponent->SetSphereRadius(100);
				});
				MultiShapeTests();
			});
			Describe("Box", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Box);
					InteractionComponent->SetBoxHalfSize(FVector(100, 100, 100));
				});
				MultiShapeTests();
			});
			Describe("Capsule", [this]()
			{
				BeforeEach([this]()
				{
					InteractionComponent->SetTraceShape(ETraceShape::Capsule);
					InteractionComponent->SetCapsuleRadius(100);
					InteractionComponent->SetCapsuleRadius(1000);
				});
				MultiShapeTests();
			});
			AfterEach([this]()
			{
				Interactable2->Destroy();
			});
		});
		LatentAfterEach(EAsyncExecution::ThreadPool, [this](const FDoneDelegate& Done)
		{
			AsyncTask(ENamedThreads::AnyThread, [this, Done]()
			{
				bool bPIEStopped;
				do
				{
					bPIEStopped = !GEditor->IsPlayingSessionInEditor();
				}
				while (bPIEStopped == false);
				Done.Execute();
			});
		});
		LatentAfterEach(EAsyncExecution::TaskGraph, [this](const FDoneDelegate& Done)
		{
			AsyncTask(ENamedThreads::GameThread, [this, Done]()
			{
				InteractionComponent->DestroyComponent();
				Actor->Destroy();
				Interactable->Destroy();
				NumActualHitResults = 0;
				ActualHitResults.Empty();
				FEndPlayMapCommand().Update();
				Done.Execute();
			});
		});
	});
	Describe("Interactable", [this]()
	{
		BeforeEach([this]()
		{
			WorldFixture = MakeUnique<FWorldFixture>();
			if(TestNotNull("World", WorldFixture->GetWorld()))
			{
				Interactable = WorldFixture->GetWorld()->SpawnActor<AInteractableTest>();
			}
		});
		Describe("Selected", [this]()
		{
			BeforeEach([this]()
			{
				IInteractable::Execute_Selected(Interactable);
			});
			It("Should Return True When Calling GetSelected", [this]()
			{
				TestTrue("GetSelected", Interactable->GetSelected());
			});
		});

		Describe("Unselected", [this]()
		{
			BeforeEach([this]()
			{
				IInteractable::Execute_Unselected(Interactable);
			});
			It("Should Return True When Calling GetUnselected", [this]()
			{
				TestTrue("GetUnselected", Interactable->GetUnselected());
			});
		});

		Describe("Use", [this]()
		{
			BeforeEach([this]()
			{
				IInteractable::Execute_Use(Interactable);
			});
			It("Should Return True When Calling GetUse", [this]()
			{
				TestTrue("GetUse", Interactable->GetUse());
			});
		});

		Describe("Alternative Use", [this]()
		{
			BeforeEach([this]()
			{
				IInteractable::Execute_AlternateUse(Interactable);
			});
			It("Should Return True When Calling GetAlternativeUse", [this]()
			{
				TestTrue("GetAlternativeUse", Interactable->GetAlternativeUse());
			});
		});

		AfterEach([this]()
		{
			WorldFixture.Reset();
		});
	});
}
#endif
