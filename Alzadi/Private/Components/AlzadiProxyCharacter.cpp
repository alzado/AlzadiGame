// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AlzadiProxyCharacter.h"

#include "Alzadi/Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AAlzadiProxyCharacter::AAlzadiProxyCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

// Called when the game starts or when spawned
void AAlzadiProxyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAlzadiProxyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (!BaseCharacterRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
			return;
		}

		if (!(BaseCharacterRef->bAIFollowActivated && BaseCharacterRef->LockedTarget))
		{
			FTransform CharTransform = BaseCharacterRef->GetTransform();
			FTransform MyTransform = GetTransform();

			FTransform Transform;
			Transform.LerpTranslationScale3D(CharTransform, MyTransform, ScalarRegister(0.5f));

			SetActorTransform(Transform);
		}
	}
}