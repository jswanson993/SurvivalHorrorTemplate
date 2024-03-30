// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	float TraceDistance = 100;
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

	
	FHitResult HitResult;
public:
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
