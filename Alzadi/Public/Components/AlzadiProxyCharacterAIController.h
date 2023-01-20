// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AlzadiProxyCharacterAIController.generated.h"

class AAlzadiProxyCharacter;

UCLASS()
class ALZADI_API AAlzadiProxyCharacterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	void TargetLoop();

	UPROPERTY(EditDefaultsOnly, Category=Target)
	float TargetLoopRate = 1.0f;

	FTimerHandle TargetTimerHandle;

	TObjectPtr<AAlzadiProxyCharacter> AlzadiProxyCharacter;
};
