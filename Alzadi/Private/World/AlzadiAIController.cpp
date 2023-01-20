// Fill out your copyright notice in the Description page of Project Settings.


#include "World/AlzadiAIController.h"

#include "NavigationSystem.h"
#include "Alzadi/Characters/BaseCharacter.h"
#include "Alzadi/Characters/Attacks/AttackComponent.h"
#include "Alzadi/Characters/Experience/ExperienceComponent.h"
#include "Alzadi/Characters/Skills/SkillComponent.h"
#include "Alzadi/Core/AlzadiGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


void AAlzadiAIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
	else
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
}

void AAlzadiAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ROLE_Authority)
	{
		AlzadiGameMode = Cast<AAlzadiGameMode>(UGameplayStatics::GetGameMode(this));
		BaseCharacterRef = Cast<ABaseCharacter>(GetPawn());

		BaseCharacterRef->SetAlzadiGameModeRef(AlzadiGameMode);
		BaseCharacterRef->SetAlzadiAIControllerRef(this);

		if (!BaseCharacterRef->Monster.DataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing DataTable in %s."), *GetName());
			return;
		}

		if (!BaseCharacterRef->ExperienceComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
			return;
		}

		if (!BaseCharacterRef->SkillComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing ExperienceComponent in %s."), *GetName());
			return;
		}

		// get all the info
		FAlzadiCharacter Monster = *BaseCharacterRef->Monster.GetRow<FAlzadiCharacter>(TEXT("lol"));

		// not in components
		BaseCharacterRef->SetCharacterType(ECharacterType::Monster);
		BaseCharacterRef->SetCharacterName(BaseCharacterRef->Monster.RowName);

		FLocation Location;
		Location.FromFVector(BaseCharacterRef->GetActorLocation());
		BaseCharacterRef->SetSpawnPoint(Location);
		BaseCharacterRef->SetAppereance(Monster.Appearence);

		// skill component
		BaseCharacterRef->SkillComponent->SetSkill(Monster.Skill);

		// attack component
		BaseCharacterRef->AttackComponent->SetBaseAttack(Monster.Weapon);
		FollowingDistanceLimitUp = BaseCharacterRef->GetAttackComponent()->GetBaseAttack().Range - 50.f;
		FollowingDistanceLimitDown = FollowingDistanceLimitUp - 50.f;

		// experience component
		BaseCharacterRef->ExperienceComponent->SetLevel(Monster.Level);
		BaseCharacterRef->ExperienceComponent->SetKillExperience(Monster.KillExperience);


		if (IsLocalController())
		{
			// targeting timer
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AAlzadiAIController::TargetLoop);
			GetWorldTimerManager().SetTimer(TargetTimerHandle, TimerDelegate, TargetLoopRate, true);
		}
		else
		{
		}
	}
	else
	{
		if (IsLocalController())
		{
		}
		else
		{
		}
	}
}

void AAlzadiAIController::TargetLoop()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
			if (!BaseCharacterRef->LockedTarget)
			{
				// stop moving
				StopMovement();

				// look for new target
				SetLockedTarget();
			}
			else
			{
				// start moving
				SetFollowLockedTarget();
			}
		}
	}
}

void AAlzadiAIController::SetLockedTarget()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
			TArray<ABaseCharacter*> PotentialTargets;

			BaseCharacterRef->GetTargeatablePlayers(PotentialTargets);

			if (PotentialTargets.Num() > 0)
			{
				BaseCharacterRef->SetLockedTarget(PotentialTargets[0]);
				UE_LOG(LogTemp, Warning, TEXT("%s locked its target"), *BaseCharacterRef->GetName());
			}
		}
	}
}

void AAlzadiAIController::SetFollowLockedTarget()
{
	if (!BaseCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing BaseCharacter in %s."), *GetName());
		return;
	}

	if (!BaseCharacterRef->AttackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing AttackComponent in %s."), *GetName());
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsLocalController())
		{
			// test if there is a locked target
			if (BaseCharacterRef->LockedTarget)
			{
				if (BaseCharacterRef->GetDistanceTo(BaseCharacterRef->LockedTarget) > FollowingDistanceLimitUp)
				{
					MoveToActor(Cast<AActor>(BaseCharacterRef->LockedTarget));
				}

				// test if is too close to move farther
				else if (BaseCharacterRef->GetDistanceTo(BaseCharacterRef->LockedTarget) <
					FollowingDistanceLimitDown)
				{
					TArray<FVector> EscapePoints;
					BaseCharacterRef->LockedTarget->GetPointsAroundCharacter(EscapePoints, 8, FollowingDistanceLimitUp);

					EscapePoints.Sort([this](FVector A, FVector B)
					{
						float DistanceA = FVector::Distance(A, this->BaseCharacterRef->GetActorLocation());
						float DistanceB = FVector::Distance(B, this->BaseCharacterRef->GetActorLocation());
						return DistanceA < DistanceB;
					});


					for (FVector EscapePoint : EscapePoints)
					{
						if (bIsPointReachable(BaseCharacterRef->GetActorLocation(), EscapePoint))
						{
							MoveToLocation(EscapePoint);
							break;
						}
					}
				}
				else
				{
					StopMovement();
				}
			}
		}
	}
}

bool AAlzadiAIController::bIsPointReachable(FVector StartPoint, FVector EndPoint)
{
	UNavigationSystemV1* AlzadiNavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(
		GetWorld());
	ANavigationData* AlzadiNavData = AlzadiNavigationSystemV1->GetNavDataForProps(
		GetNavAgentPropertiesRef());

	if (AlzadiNavData)
	{
		TSubclassOf<UNavigationQueryFilter> FilterClass = UNavigationQueryFilter::StaticClass();
		FSharedConstNavQueryFilter QueryFilter = UNavigationQueryFilter::GetQueryFilter(
			*AlzadiNavData, FilterClass);

		FPathFindingQuery MyAIQuery = FPathFindingQuery(
			this, *AlzadiNavData, StartPoint, EndPoint, QueryFilter);

		return AlzadiNavigationSystemV1->TestPathSync(MyAIQuery);
	}

	return false;
}
