// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AlzadiAIController.generated.h"

class AAlzadiGameMode;
class ABaseCharacter;

UCLASS()
class ALZADI_API AAlzadiAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	TObjectPtr<AAlzadiGameMode> AlzadiGameMode;

	TObjectPtr<ABaseCharacter> BaseCharacterRef;

	// target
	void TargetLoop();
	FTimerHandle TargetTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category=Target)
	float TargetLoopRate = 1.f;

	// set target
	void SetLockedTarget();

	// follow target
	void SetFollowLockedTarget();

	float FollowingDistanceLimitUp = 0.f;
	float FollowingDistanceLimitDown = 0.f;

	bool bIsPointReachable(FVector StartPoint, FVector EndPoint);
};
