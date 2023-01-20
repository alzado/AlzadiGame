// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AlzadiProxyCharacterAIController.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Components/AlzadiProxyCharacter.h"


void AAlzadiProxyCharacterAIController::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TargetTimerDelegate = FTimerDelegate::CreateUObject(
		this, &AAlzadiProxyCharacterAIController::TargetLoop);
	GetWorldTimerManager().SetTimer(TargetTimerHandle, TargetTimerDelegate, TargetLoopRate, true);
}

void AAlzadiProxyCharacterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AlzadiProxyCharacter = Cast<AAlzadiProxyCharacter>(GetPawn());
}

void AAlzadiProxyCharacterAIController::TargetLoop()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (!AlzadiProxyCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiProxyCharacter in %s."), *GetName());
			return;
		}

		if (!AlzadiProxyCharacter->BaseCharacterRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
			return;
		}

		if (AlzadiProxyCharacter->BaseCharacterRef->bAIFollowActivated)
		{
			if (!AlzadiProxyCharacter->BaseCharacterRef->LockedTarget)
			{
				// stop moving
				StopMovement();
				AlzadiProxyCharacter->BaseCharacterRef->bIsVisible = true;
			}
			else
			{
				// start moving
				MoveToActor(Cast<AActor>(AlzadiProxyCharacter->BaseCharacterRef->LockedTarget));
				AlzadiProxyCharacter->BaseCharacterRef->bIsVisible = false;
			}
		}
	}
}
