// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractionComponent.generated.h"

class UArrowComponent;
class UGizmoArrowComponent;

UENUM()
enum ETraceShape
{
	Line,
	Box,
	Capsule,
	Sphere
};

UENUM()
enum ETraceStyle
{
	Single,
	Multi
};



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ETraceStyle> TraceStyle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trace|Shape", meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ETraceShape> TraceShape; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	float TraceDistance = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<ECollisionChannel> TraceChannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Trace, meta=(AllowPrivateAccess="true"))
	bool bDrawDebugLine = true;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Trace, meta=(AllowPrivateAccess="true"))
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trace|Shape|Box", meta=(AllowPrivateAccess="true"))
	FVector BoxHalfSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trace|Shape|Capsule", meta=(AllowPrivateAccess="true"))
	float CapsuleRadius = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trace|Shape|Capsule", meta=(AllowPrivateAccess="true"))
	float CapsuleHalfHeight = 60;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trace|Shape|Sphere", meta=(AllowPrivateAccess="true"))
	float SphereRadius = 100;
	FHitResult HitResult;
	TArray<FHitResult> HitResults;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Editor",meta=(AllowPrivateAccess="true"))
	UArrowComponent* ArrowComponent;
	
	UPROPERTY()
	TArray<AActor*> HitActors;

	bool bIsUpdatingActors = false;
public:
	// Sets default values for this component's properties
	UInteractionComponent();

private:
	void PerformTrace();
	void TraceSingle();
	void TraceMulti();
	FCollisionShape GetCollisionShapeFromTraceShape(ETraceShape Shape);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintSetter)
	void SetTraceStyle(ETraceStyle NewTraceStyle) { TraceStyle = NewTraceStyle; }
	UFUNCTION(BlueprintSetter)
	void SetTraceShape(ETraceShape NewTraceShape) { TraceShape = NewTraceShape; }
	UFUNCTION(BlueprintSetter)
	void SetTraceDistance(float NewDistance) { TraceDistance = NewDistance; }
	UFUNCTION(BlueprintSetter)
	void SetTraceTypeQuery(ECollisionChannel NewTraceChannel) { TraceChannel = NewTraceChannel; }
	UFUNCTION(BlueprintSetter)
	void SetDrawDebugLine(bool NewDrawDebugLine) { this->bDrawDebugLine = NewDrawDebugLine; }
	UFUNCTION(BlueprintSetter)
	void SetActorsToIgnore(const TArray<AActor*>& NewActorsToIgnore) { this->ActorsToIgnore = NewActorsToIgnore; }
	UFUNCTION(BlueprintSetter)
	void SetBoxHalfSize(const FVector& NewBoxHalfSize) { this->BoxHalfSize = NewBoxHalfSize; }
	UFUNCTION(BlueprintSetter)
	void SetCapsuleRadius(float NewCapsuleRadius) { this->CapsuleRadius = NewCapsuleRadius; }
	UFUNCTION(BlueprintSetter)
	void SetCapsuleHalfHeight(float NewCapsuleHalfHeight) { this->CapsuleHalfHeight = NewCapsuleHalfHeight; }
	UFUNCTION(BlueprintSetter)
	void SetSphereRadius(float NewSphereRadius) { this->SphereRadius = NewSphereRadius; }

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetInteractables();

};
