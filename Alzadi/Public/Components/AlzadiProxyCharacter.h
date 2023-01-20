// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlzadiProxyCharacter.generated.h"

class AAlzadiProxyCharacterAIController;
class ABaseCharacter;

class UCapsuleComponent;

UCLASS()
class ALZADI_API AAlzadiProxyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAlzadiProxyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	TObjectPtr<ABaseCharacter> BaseCharacterRef;
};
