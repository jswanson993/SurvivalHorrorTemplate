#if WITH_AUTOMATION_TESTS
#include "HealthComponent.h"
#include "HealthComponentTestActor.h"
#include "Engine/DamageEvents.h"
#include "Tests/WorldFixture.h"
#include "Misc/AutomationTest.h"



BEGIN_DEFINE_SPEC(FHealthComponent, "Health.HealthComponent",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
TUniquePtr<FWorldFixture> WorldFixture;
AHealthComponentTestActor* Actor;
UHealthComponent* HealthComponent;
FPointDamageEvent PointDamageEvent;
float Health;
float Damage;
float HealAmount;
void Setup()
{
	WorldFixture = MakeUnique<FWorldFixture>();
	if(TestNotNull("World", WorldFixture->GetWorld()))
	{
		Actor = WorldFixture->GetWorld()->SpawnActor<AHealthComponentTestActor>();
		HealthComponent = NewObject<UHealthComponent>(Actor);
		TestNotNull("MyActor", Actor);
		
		Health = HealthComponent->GetCurrentHealth();
		HealthComponent->OnHealthChanged.AddDynamic(Actor, &AHealthComponentTestActor::HealthChanged);
		HealthComponent->OnDeath.AddDynamic(Actor, &AHealthComponentTestActor::Dead);
		Actor->OnTakeAnyDamage.AddDynamic(HealthComponent, &UHealthComponent::TakeDamage);
		HealAmount = 10;
		Damage = 0;
	}
}

void Teardown()
{
	WorldFixture.Reset();
}

END_DEFINE_SPEC(FHealthComponent)

void FHealthComponent::Define()
{
	Describe("Initialize", [this]()
	{
		BeforeEach([this]()
		{
			Setup();
		});
		
		It("Should Be Equal To Max Health", [this]()
		{
			TestEqual(TEXT("CurrentHealth"), HealthComponent->GetCurrentHealth(),
				HealthComponent->GetMaxHealth());
		});
		
		AfterEach([this]()
		{
			Teardown();
		});
	});
	Describe("Take Damage", [this]()
	{
		BeforeEach([this]()
		{
			Setup();
			Damage = 10;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
		});
		
		It("Should set health equal to health minus damage when taking damage", [this]()
		{
			TestEqual(FString::Printf(TEXT("GetCurrentHealth = Health: %f - Damage: %f"), Health, Damage),
			          HealthComponent->GetCurrentHealth(), Health - Damage);
		});

		It("Should set health amount changed to negative damage when taking damage", [this]()
		{
			TestEqual(TEXT("GetHealthAmountChanged"),
			          Actor->GetAmountHealthChanged(), -Damage);
		});

		It("Should not set current health below minimum health when taking damage greater than current health", [this]()
		{
			Damage = Health + 10;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);

			TestEqual(TEXT("GetCurrentHealth"),
			          HealthComponent->GetCurrentHealth(), HealthComponent->GetMinHealth());
		});

		It("Should not increase current health when taking negative damage", [this]()
		{
			const float NegativeDamage = -1;
			Actor->TakeDamage(NegativeDamage, PointDamageEvent, nullptr, nullptr);
			TestEqual(TEXT("GetCurrentHealth"), HealthComponent->GetCurrentHealth(), Health - Damage);
		});
		
		AfterEach([this]()
		{
			Teardown();
		});
	});

	Describe("Add Health", [this]()
	{
		BeforeEach([this]()
		{
			Setup();
		});

		It("Should set current health back to max health when healing for same amount of damage taken", [this]()
		{
			Damage = 10;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
			HealthComponent->AddHealth(HealAmount);
			TestEqual(FString::Printf(TEXT("GetCurrentHealth = Health: %f - Damage: %f + HealAmount: %f"), Health, Damage, HealAmount),
				HealthComponent->GetCurrentHealth(), Health - Damage + HealAmount);
		});

		It("Should increase current health when healing after taking damage", [this]()
		{
			Damage = 10;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
			HealAmount = 5;
			HealthComponent->AddHealth(HealAmount);
			TestEqual(FString::Printf(TEXT("GetCurrentHealth = Health: %f - Damage: %f + HealAmount: %f"), Health, Damage, HealAmount),
				HealthComponent->GetCurrentHealth(), Health - Damage + HealAmount);
		});

		It("Should not increase health past max health when healing for more than max health", [this]()
		{
			HealAmount = 1;
			HealthComponent->AddHealth(HealAmount);
			TestEqual(FString::Printf(TEXT("GetCurrentHealth = GetMaxHealth: %f != Health: %f + HealthAmount: %f"),
				HealthComponent->GetMaxHealth(), Health, HealAmount), HealthComponent->GetCurrentHealth(), Health);
		});

		It("Should not decrease health when receiving negative health amount", [this]()
		{
			Damage = 10;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
			HealAmount = -5;
			HealthComponent->AddHealth(HealAmount);
			TestNotEqual(FString::Printf(TEXT("GetCurrentHealth != Health: %f - Damage: %f + HealAmount: %f"), Health, Damage, HealAmount),
				HealthComponent->GetCurrentHealth(), Health - Damage + HealAmount);
			TestEqual(FString::Printf(TEXT("GetCurrentHealth = Health: %f - Damage: %f"), Health, Damage),
				HealthComponent->GetCurrentHealth(), Health - Damage);
		});
		It("Should set health amount changed to positive value when healing damage", [this]()
		{
			Damage = 10;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
			HealAmount = 5;
			HealthComponent->AddHealth(HealAmount);
			TestEqual(TEXT("HealthAmountChanged"), Actor->GetAmountHealthChanged(), HealAmount);
		});
		
		AfterEach([this]()
		{
			Teardown();
		});
	});

	Describe("On Death", [this]()
	{
		BeforeEach([this]()
		{
			Setup();
		});

		It("Should send notification when health reaches min health", [this]()
		{
			Damage = Health + 1;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
			TestTrue("Actor is dead", Actor->GetIsDead());
		});
		
		It("Should not send notification when health does not reach min health", [this]()
		{
			Damage = 1;
			Actor->TakeDamage(Damage, PointDamageEvent, nullptr, nullptr);
			TestFalse("Actor is dead", Actor->GetIsDead());
		});

		AfterEach([this]()
		{
			Teardown();
		});
	});
	
}

#endif
