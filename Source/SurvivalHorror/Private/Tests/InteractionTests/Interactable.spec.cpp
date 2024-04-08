#include "Tests/AutomationEditorCommon.h"
#if WITH_AUTOMATION_TESTS
#include "InteractableTest.h"
#include "InteractionComponent.h"
#include "InteractionTestActor.h"
#include "Tests/WorldFixture.h"
#include "Misc/AutomationTest.h"



BEGIN_DEFINE_SPEC(FInteractComponent, "Interact.InteractComponent",
								 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
UWorld* World;
AInteractableTest* Interactable;
AInteractionTestActor* Actor;
UInteractionComponent* InteractionComponent;
void Setup()
{
	FEditorLoadMap(FString("/Game/Tests/TestMaps/VoidWorld"));
	FStartPIECommand(true);
	World = GEditor->GetPIEWorldContext()->World();
	Interactable = World->SpawnActor<AInteractableTest>();
	Actor = World->SpawnActor<AInteractionTestActor>();
	InteractionComponent = Actor->GetInteractionComponent();
}

void Teardown()
{
	World->BeginTearingDown();
	for(auto It = TActorIterator<AActor>(World); It; ++It)
	{
		It->Destroy();
	}

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
}

END_DEFINE_SPEC(FInteractComponent);

DEFINE_LATENT_AUTOMATION_COMMAND(OnExecuteComplete);


void FInteractComponent::Define()
{
	Describe("Single Trace", [this]()
	{
		BeforeEach([this]()
		{
			Setup();
			UE_LOG(LogTemp, Warning,TEXT("Single Trace"))
		});
		Describe("Blocking Hits", [this](){

			LatentBeforeEach(EAsyncExecution::Thread, [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::GameThread, [&]()
				{
					Setup();
					Actor->SetActorLocation(FVector::ZeroVector);
					const FVector StartLocation = FVector(10, 0, 0);
					Interactable->SetActorLocation(StartLocation);
					Done.Execute();
				});
			});
			
			LatentIt("Should return hit when interactable actor is in front of line trace", [this](const FDoneDelegate& Done)
			{
				AsyncTask(ENamedThreads::GameThread, [&]()
				{
					if(GEditor->IsPlayingSessionInEditor())
					{
						if(InteractionComponent != nullptr)
						{
							const TArray<FHitResult> HitResults = InteractionComponent->GetHitResults();
							if(HitResults.Num() == 0)
							{
								return;
							}

							TestEqual(TEXT("Single trace should return one hit result when there is an actor in front of trace"),
							HitResults.Num(), 1);
							if(InteractionComponent->GetHitResults().Num() > 0)
							{
								Done.Execute();
							}
						}
					}
				});
				
			});
			





		});

		AfterEach([this]()
		{
			Teardown();
		});

	});
}


#endif