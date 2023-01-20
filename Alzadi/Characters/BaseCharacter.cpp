// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Alzadi/Core/AlzadiPlayerController.h"
#include "Alzadi/Core/Definitions.h"
#include "Attacks/AttackComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AlzadiProxyCharacter.h"
#include "Components/AlzadiProxyCharacterAIController.h"
#include "Components/CharacterWidgetComponent.h"
#include "Components/WidgetComponent.h"
#include "Experience/ExperienceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Health/HealthComponent.h"
#include "Inventory/EquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Skills/SkillComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// mesh
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// camera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = 800.f;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// Crossair targeting
	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Component"));
	PlaneComponent->SetupAttachment(RootComponent);
	PlaneComponent->SetCanEverAffectNavigation(false);
	PlaneComponent->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	PlaneComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundObject(
		TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (FoundObject.Succeeded())
	{
		PlaneComponent->SetStaticMesh(FoundObject.Object);
	}

	// Experience and Level
	ExperienceComponent = CreateDefaultSubobject<UExperienceComponent>(TEXT("Experience Component"));
	ExperienceComponent->SetBaseCharacter(this);

	// Health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->SetBaseCharacter(this);

	CharacterWidgetComponent = CreateDefaultSubobject<UCharacterWidgetComponent>(TEXT("Widget Component"));
	CharacterWidgetComponent->SetBaseCharacterReference(this);
	CharacterWidgetComponent->SetupAttachment(RootComponent);

	// Skills
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("Skill Component"));
	SkillComponent->SetBaseCharacter(this);

	// Inventory
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Items Component"));
	EquipmentComponent->SetBaseCharacter(this);

	// Attack
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	AttackComponent->SetBaseCharacter(this);

	// Configure character movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, AlzadiProxyCharacter);
	DOREPLIFETIME(ABaseCharacter, Appearence);
	DOREPLIFETIME(ABaseCharacter, bIsVisible);
	DOREPLIFETIME(ABaseCharacter, CharacterName);
	DOREPLIFETIME(ABaseCharacter, LockedTarget);
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		// delete locked target if not in range
		if (LockedTarget)
		{
			if (GetDistanceTo(LockedTarget) > LockedTargetMinimumDistance)
			{
				ResetLockedTarget();
			}
		}

		if (IsPlayerControlled())
		{
			if (!AlzadiProxyCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiProxyCharacter in %s."), *GetName());
				return;
			}

			// follow AI path
			if (LockedTarget && bAIFollowActivated)
			{
				FTransform CharTransform = AlzadiProxyCharacter->GetTransform();
				FTransform MyTransform = GetTransform();

				FTransform Transform;
				Transform.LerpTranslationScale3D(CharTransform, MyTransform, ScalarRegister(0.5f));

				SetActorTransform(Transform);
			}
		}
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (Controller->IsPlayerController())
		{
			if (!AlzadiProxyCharacterClass)
			{
				UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiProxyCharacterClass in %s."), *GetName());
				return;
			}

			if (!AlzadiProxyCharacterAIControllerClass)
			{
				UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiProxyCharacterAIControllerClass in %s."), *GetName());
				return;
			}

			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();

			AlzadiProxyCharacterAIController = GetWorld()->SpawnActor<
				AAlzadiProxyCharacterAIController>(AlzadiProxyCharacterAIControllerClass, Location, Rotation);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.Instigator = GetInstigator();
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AlzadiProxyCharacter = GetWorld()->SpawnActor<AAlzadiProxyCharacter>(
				AlzadiProxyCharacterClass, Location, Rotation,
				SpawnParameters);

			AlzadiProxyCharacterAIController->Possess(AlzadiProxyCharacter);
			AlzadiProxyCharacter->BaseCharacterRef = this;
		}
	}
}

void ABaseCharacter::SetbIsVisible(bool InValue)
{
	bIsVisible = InValue;

	OnbIsVisibleUpdate();
}

void ABaseCharacter::OnRep_bIsVisible()
{
	OnbIsVisibleUpdate();
}

void ABaseCharacter::OnbIsVisibleUpdate()
{
	if (IsPlayerControlled())
	{
		if (!AlzadiProxyCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiProxyCharacter in %s."), *GetName());
			return;
		}

		AlzadiProxyCharacter->GetMesh()->SetVisibility(!bIsVisible);
		GetMesh()->SetVisibility(bIsVisible);
	}
}

void ABaseCharacter::SetAlzadiGameModeRef(AAlzadiGameMode* GameMode)
{
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing GameMode in %s."), *GetName());
		return;
	}

	AlzadiGameModeRef = GameMode;
}

void ABaseCharacter::SetAlzadiPlayerControllerRef(AAlzadiPlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing PlayerController in %s."), *GetName());
		return;
	}

	AlzadiPlayerControllerRef = PlayerController;
}

void ABaseCharacter::SetAlzadiAIControllerRef(AAlzadiAIController* AIController)
{
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AIController in %s."), *GetName());
		return;
	}

	AlzadiAIControllerRef = AIController;
}

void ABaseCharacter::SetCharacterType(ECharacterType InCharacterType)
{
	CharacterType = InCharacterType;
}

ECharacterType ABaseCharacter::GetCharacterType()
{
	return CharacterType;
}

void ABaseCharacter::SetAppereance(FAppearence InAppereance)
{
	Appearence = InAppereance;

	OnAppereanceUpdate();
}

void ABaseCharacter::OnRep_Appereance()
{
	OnAppereanceUpdate();
}

void ABaseCharacter::OnAppereanceUpdate()
{
	if (!Appearence.SkeletalMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing SkeletalMesh in %s."), *GetName());
		return;
	}

	if (!Appearence.AnimBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AnimBlueprint in %s."), *GetName());
		return;
	}

	GetMesh()->SetSkeletalMesh(Appearence.SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(Appearence.AnimBlueprint->GeneratedClass);
}

void ABaseCharacter::GetTargeatablePlayers(TArray<ABaseCharacter*>& OutPlayers)
{
	// get actor location
	FVector Location = GetActorLocation();

	// declare sphere
	FCollisionShape ColSphere = FCollisionShape::MakeSphere(LockedTargetMinimumDistance);

	// declare array of actors
	TArray<FHitResult> OutHitResults;

	// find first traced target
	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHitResults, Location, Location, FQuat::Identity,
	                                              ECC_Pawn, ColSphere);

	if (bIsHit)
	{
		for (FHitResult& HitResult : OutHitResults)
		{
			if (Cast<ABaseCharacter>(HitResult.GetActor()))
			{
				if (Cast<ABaseCharacter>(HitResult.GetActor()) != this)
				{
					if (Cast<ABaseCharacter>(HitResult.GetActor())->CharacterType == ECharacterType::Player)
					{
						OutPlayers.Add(Cast<ABaseCharacter>(HitResult.GetActor()));
					}
				}
			}
		}
	}
}

void ABaseCharacter::GetPointsAroundCharacter(TArray<FVector>& OutPoints, int32 NumberOfPoints,
                                              float DistanceFromCharacter)
{
	FVector PointProjection = FVector(DistanceFromCharacter, 0.f, 0.f);

	for (int32 Index = 0; Index < NumberOfPoints; ++Index)
	{
		PointProjection = PointProjection.RotateAngleAxis(360.f / NumberOfPoints, FVector(0.f, 0.f, 1.f));
		OutPoints.Add(PointProjection + GetActorLocation());
	}
}

void ABaseCharacter::SetLockedTarget(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	LockedTarget = Character;

	LockedTarget->LockedBy.Add(this);
}

void ABaseCharacter::HandleDeath()
{
	if (!ExperienceComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
		return;
	}

	if (!HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing HealthComponent in %s."), *GetName());
		return;
	}

	HideCharacter(true);

	// server is in charge of death handling
	if (GetLocalRole() == ROLE_Authority)
	{
		// teleport a la csm
		SetActorLocation(FVector(0.f, 0.f, 10000.f));

		// give experience to killers
		ExperienceComponent->GiveExperienceWhenKilled();

		// reset attackers damage
		HealthComponent->ResetAttackerDamage();

		// reset targets
		ResetLockedTarget();
		ResetCharacterAsLockedTarget();

		if (IsLocallyControlled())
		{
			// for non players

			// restore health
			HealthComponent->SetHealth(HealthComponent->GetMaxHealth());

			// set spawn timer
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABaseCharacter::SpawnCharacter);
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, SpawnRate, false);
		}
		else
		{
			// for players
			if (!AlzadiPlayerControllerRef)
			{
				UE_LOG(LogTemp, Warning, TEXT("Missing AlzadiPlayerController in %s."), *GetName());
				return;
			}

			// loose experience
			ExperienceComponent->SetExperience(
				ExperienceComponent->GetExperience() * (1 - ExperienceComponent->ExperienceLossOnDeath));

			// restore health
			HealthComponent->SetMaxHealth(ExperienceComponent->GetLevel(), 0.f);
			HealthComponent->SetHealth(HealthComponent->GetMaxHealth());

			// teleport to spawnpoint
			SetActorLocation(SpawnPoint.ToFVector());

			// logout
			AlzadiPlayerControllerRef->CharacterLogout();
		}
	}
	else
	{
		if (IsLocallyControlled())
		{
		}
		else
		{
		}
	}
}

void ABaseCharacter::HideCharacter(bool bHideCharacter)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SetActorEnableCollision(!bHideCharacter);
		SetActorHiddenInGame(bHideCharacter);
		SetActorTickEnabled(!bHideCharacter);

		if (IsLocallyControlled())
		{
		}
		else
		{
		}
	}
	else
	{
		if (IsLocallyControlled())
		{
		}
		else
		{
		}
	}

	// called everywhere

	if (bHideCharacter)
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1.f;
	}
}

void ABaseCharacter::SpawnCharacter()
{
	// teleport to spawnpoint
	SetActorLocation(SpawnPoint.ToFVector());

	HideCharacter(false);
}

void ABaseCharacter::SetSpawnPoint(FLocation Location)
{
	SpawnPoint = Location;
}

FLocation ABaseCharacter::GetSpawnPoint()
{
	return SpawnPoint;
}

void ABaseCharacter::SetCharacterName(FName Name)
{
	CharacterName = Name;
}

FName ABaseCharacter::GetCharacterName()
{
	return CharacterName;
}

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		bAIFollowActivated = false;
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		bAIFollowActivated = false;
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::HandleDestruction(FData Data)
{
	// SetActorLocation(FVector(0.0f, 0.0f, 10000.0f));
	ResetLockedTarget();
	ResetCharacterAsLockedTarget();
	AlzadiProxyCharacter->Destroy();
	AlzadiProxyCharacterAIController->Destroy();
	Destroy();
}

void ABaseCharacter::SetLockedTargetServer_Implementation(ABaseCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Character in %s."), *GetName());
		return;
	}

	LockedTarget = Character;

	LockedTarget->LockedBy.Add(this);
}

bool ABaseCharacter::SetLockedTargetServer_Validate(ABaseCharacter* Character)
{
	return true;
}

void ABaseCharacter::ResetLockedTarget_Implementation()
{
	// remove locked target
	if (LockedTarget)
	{
		LockedTarget->LockedBy.Remove(this);
		LockedTarget = nullptr;
	}
}

bool ABaseCharacter::ResetLockedTarget_Validate()
{
	return true;
}

void ABaseCharacter::ResetCharacterAsLockedTarget()
{
	// remove locked target from attackers
	for (ABaseCharacter*& Attacker : LockedBy)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s unlocked its target"), *Attacker->GetName());
		Attacker->LockedTarget = nullptr;
	}

	// clear locked by
	LockedBy.Empty();
}
