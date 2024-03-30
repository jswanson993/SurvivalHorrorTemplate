// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
}


void UHealthComponent::AddHealth(const float Health)
{
	if(Health <= 0)
	{
		return;
	}
	CurrentHealth = FMath::Clamp(CurrentHealth + Health, MinHealth, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float DamageValue, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(DamagedActor == GetOwner() && DamageValue >= 0)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - DamageValue, MinHealth, MaxHealth);
		OnHealthChanged.Broadcast(-DamageValue);
		if(CurrentHealth <= MinHealth)
		{
			OnDeath.Broadcast(DamageType, InstigatedBy, DamageCauser);
		}
	}
}

