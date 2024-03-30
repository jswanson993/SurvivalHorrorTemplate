#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "HealthComponent.h"
#include "HealthComponentTestActor.h"
#include "Engine/DamageEvents.h"
#include "Tests/AutomationEditorCommon.h"


///////////////////////Test Setup////////////////////////////////////////////////
UWorld* World;
AHealthComponentTestActor* Actor;
UHealthComponent* HealthComp;
FPointDamageEvent PointDamageEvent;

void Setup()
{
	Actor = World->SpawnActor<AHealthComponentTestActor>();
	HealthComp = NewObject<UHealthComponent>(Actor);
}

void TearDown()
{
	HealthComp->DestroyComponent();
	//Actor->Destroy();
}

//////////Begin Tests//////////////////////////////////////////////

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponent_Initilization, "Health.HealthComponent.Initialization",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHealthComponent_Initilization::RunTest(const FString& Parameters)
{
	//Current health should start at max health
	{
		HealthComp = NewObject<UHealthComponent>();

		if(HealthComp->GetCurrentHealth() != HealthComp->GetMaxHealth())
		{
			AddError(TEXT("Health Component should start at max health"));
		}
		HealthComp->DestroyComponent();
	}
	
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponent_TakeDamage, "Health.HealthComponent.TakeDamage",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHealthComponent_TakeDamage::RunTest(const FString& Parameters)
{
	World = FAutomationEditorCommonUtils::CreateNewMap();

	//Current Health should equal the difference between current health and damage
	{
		Setup();
		const float Health = HealthComp->GetCurrentHealth();
		const float Damage = 10;

		Actor->OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::TakeDamage);
		Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
		
		TestEqual(TEXT("Current health should be health minus damage when taking damange"),
			HealthComp->GetCurrentHealth(), Health - Damage);
		
		TearDown();
	}

	//Current Health should equal the difference between current health and damage
	{
		Setup();
		const float Damage = 10;

		HealthComp->OnHealthChanged.AddDynamic(Actor, &AHealthComponentTestActor::HealthChanged);
		Actor->OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::TakeDamage);
		Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
		
		TestEqual(TEXT("Health amount changed should be negative damage value"),
			Actor->GetAmountHealthChanged(), -Damage);
		
		TearDown();
	}

	//Current health should not go below min health
	{
		Setup();
		const float Damage = HealthComp->GetCurrentHealth() + 1;
		Actor->OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::TakeDamage);
		Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);

		TestEqual(TEXT("Current health should not go below min health when damage is greater than current health"),
			HealthComp->GetCurrentHealth(), HealthComp->GetMinHealth());
		TearDown();
	}

	//Current health should not increase when taking negative damage
	{
		Setup();
		const float Health = HealthComp->GetCurrentHealth();
		const float Damage = 10;
		Actor->OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::TakeDamage);
		Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);

		float NegativeDamage = -1;
		Actor->TakeDamage(NegativeDamage, PointDamageEvent, nullptr, nullptr);
		TestEqual(TEXT("Current health should not increase when taking negative damage"), HealthComp->GetCurrentHealth(), Health - Damage);

		TearDown();
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponent_AddHealth, "Health.HealthComponent.AddHealth",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHealthComponent_AddHealth::RunTest(const FString& Parameters)
{
	World = FAutomationEditorCommonUtils::CreateNewMap();
	//Current health should be the sum of current health and heal amount 
	{
		Setup();
		const float Health = HealthComp->GetCurrentHealth();
		const float Damage = 10;
		
		Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
		const float HealAmount = 10;
		HealthComp->AddHealth(HealAmount);
		TestEqual(TEXT("Current health must be equal to starting health minus damage plus amount healed"), HealthComp->GetCurrentHealth(), Health - Damage + HealAmount);
		TearDown();
	}
	//Current health should be the sum of current health and heal amount 
	{
		Setup();
		const float Damage = 20;
		HealthComp->OnHealthChanged.AddDynamic(Actor, &AHealthComponentTestActor::HealthChanged);
		Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
		const float HealAmount = 10;
		HealthComp->AddHealth(HealAmount);
		TestEqual(TEXT("Health amount changed should be equal to heal amount"), Actor->GetAmountHealthChanged(),  HealAmount);
		TearDown();
	}

	//Current health should not be greater than max health
	{
		Setup();
		const float HealAmount = 10;
		HealthComp->AddHealth(HealAmount);
		TestFalse(TEXT("Current health must be less than or equal to the max health"), HealthComp->GetCurrentHealth() > HealthComp->GetMaxHealth());
		TearDown();
	}

	//Current health should not decrease
	{
		Setup();
		const float HealAmount = -1;
		HealthComp->AddHealth(HealAmount);
		TestFalse(TEXT("Current Health should not decrease when heal amount is negative"), HealthComp->GetCurrentHealth() < HealthComp->GetMinHealth());
		TearDown();
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthComponent_OnDeath, "Health.HealthComponent.OnDeath",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHealthComponent_OnDeath::RunTest(const FString& Parameters)
{
	World = FAutomationEditorCommonUtils::CreateNewMap();
	World->InitializeActorsForPlay({});
	//OnDeath delegate should broadcast when actor health reaches minimum
	{
		Setup();
		HealthComp->OnDeath.AddDynamic(Actor, &AHealthComponentTestActor::Dead);
		Actor->OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::TakeDamage);
		Actor->TakeDamage(HealthComp->GetCurrentHealth() + 1, PointDamageEvent, nullptr, nullptr);
		TestTrue(TEXT("OnDeath delegate should broadcast when actor health reaches minimum"), Actor->GetIsDead());
		TearDown();
	}
	//OnDeath delegate should not broadcast when actor health doesn't reach minimum
	{
		Setup();
		HealthComp->OnDeath.AddDynamic(Actor, &AHealthComponentTestActor::Dead);
		Actor->OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::TakeDamage);
		Actor->TakeDamage(1, PointDamageEvent, nullptr, nullptr);
		TestFalse(TEXT("OnDeath delegate should not broadcast when actor health reach minimum"), Actor->GetIsDead());
		TearDown();
	}
	
	return true;
}

#endif