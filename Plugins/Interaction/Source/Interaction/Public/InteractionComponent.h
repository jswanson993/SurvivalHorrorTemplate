// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractionComponent.generated.h"

UENUM()
enum ETraceStyle
{
	Single,
	Multi
};

//Create Delegate To Broadcast hit actors and components

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ETraceStyle> TraceStyle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ECollisionShape::Type> TraceShape;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	float TraceDistance = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	bool bDrawDebugLine = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	FColor TraceColor = FColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	FColor TraceHitColor = FColor::Green;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Trace, meta=(AllowPrivateAccess="true"))
	TArray<AActor*> ActorsToIgnore;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::Type::ForOneFrame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	float DrawTime = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Trace|Shape|Box", meta=(AllowPrivateAccess="true"))
	FVector BoxHalfSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Trace|Shape|Capsule", meta=(AllowPrivateAccess="true"))
	float CapsuleRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Trace|Shape|Capsule", meta=(AllowPrivateAccess="true"))
	float CapsuleHalfHeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Trace|Shape|Sphere", meta=(AllowPrivateAccess="true"))
	float SphereRadius;
	FHitResult HitResult;
	TArray<FHitResult> HitResults;
	
	UPROPERTY()
	TArray<AActor*> HitActors;
public:
	// Sets default values for this component's properties
	UInteractionComponent();

private:
	void PerformTrace();
	void TraceSingle();
	void TraceMulti();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateHitActors();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FHitResult> GetHitResults();

};
