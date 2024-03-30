// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponentTestActor.generated.h"

UCLASS(Hidden, ClassGroup=(Testing))
class SURVIVALHORROR_API AHealthComponentTestActor : public AActor
{
	GENERATED_BODY()
	
	bool IsDead = false;
	float AmountHealthChanged = 0;
public:
	UFUNCTION(CallInEditor)
	void Dead(const UDamageType* DamageType, AController* InstigatedBy, AActor* Killer)
	{
		IsDead = true;
	}
	UFUNCTION(CallInEditor)
	void HealthChanged(float AmountChanged)
	{
		AmountHealthChanged = AmountChanged;
	}
	bool GetIsDead() { return IsDead; }
	float GetAmountHealthChanged() {return AmountHealthChanged; }
};
