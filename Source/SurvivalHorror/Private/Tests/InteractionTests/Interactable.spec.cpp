#include "InteractableTest.h"
#include "InteractionComponent.h"
#include "Tests/AutomationCommon.h"
#if WITH_AUTOMATION_TESTS
#include "Tests/WorldFixture.h"
#include "Misc/AutomationTest.h"

BEGIN_DEFINE_SPEC(FInteractComponent, "Interact.InteractComponent",
								 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
TUniquePtr<FWorldFixture> WorldFixture;
AInteractableTest* Interactable;
AActor* Actor;
UInteractionComponent* InteractionComponent;
void Setup()
{
	WorldFixture = MakeUnique<FWorldFixture>();
	if(TestNotNull("World", WorldFixture->GetWorld()))
	{
		Interactable = WorldFixture->GetWorld()->SpawnActor<AInteractableTest>();
		Actor = WorldFixture->GetWorld()->SpawnActor<AActor>();
		InteractionComponent = NewObject<UInteractionComponent>(Actor);
	}
}

void Teardown()
{
	WorldFixture.Reset();
}

END_DEFINE_SPEC(FInteractComponent);
void FInteractComponent::Define()
{
	Describe("Single Trace", [this]()
	{
		BeforeEach([this]()
		{
			Setup();
		});
		Describe("Blocking Hits", [this](){
			BeforeEach([this]()
			{
				Actor->SetActorLocation(FVector::ZeroVector);
				const FVector StartLocation = FVector(10, 0, 0);
				Interactable->SetActorLocation(StartLocation);
			});
			LatentIt("Should return hit when interactable actor is in front of line trace", [this](const FDoneDelegate& Done)
			{
				FWaitLatentCommand(1);
				TestTrue("GetHitResults.Num > 0", InteractionComponent->GetHitResults().Num() > 0);
				Done.Execute();
			});
		});

		AfterEach([this]()
		{
			Teardown();
		});

	});
}


#endif