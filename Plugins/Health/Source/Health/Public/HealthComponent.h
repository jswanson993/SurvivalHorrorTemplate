// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeathSignature, const UDamageType*,
	DamageType, class AController*, InstigatedBy,  AActor*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, AmountChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEALTH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health, meta=(AllowPrivateAccess="true"))
	float MaxHealth = 100;
	//Health value which the actor is considered dead
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health, meta=(AllowPrivateAccess="true"))
	float MinHealth = 0;
	UPROPERTY(BlueprintReadOnly, Category=Health, meta=(AllowPrivateAccess="true"))
	float CurrentHealth;


public:
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintGetter)
	float GetCurrentHealth(){return CurrentHealth;}
	UFUNCTION(BlueprintGetter)
	float GetMinHealth(){return MinHealth;}
	UFUNCTION(BlueprintGetter)
	float GetMaxHealth(){return MaxHealth;}
	void AddHealth(float Health);

	UFUNCTION(CallInEditor)
	void TakeDamage(AActor* DamagedActor, float DamageValue, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
