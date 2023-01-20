// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Alzadi/Core/Definitions.h"
#include "BaseCharacter.generated.h"

class AAlzadiProxyCharacter;
class AAlzadiProxyCharacterAIController;
class AAlzadiAIController;
class AAlzadiGameMode;
class AAlzadiPlayerController;
class UAnimBlueprint;
class UAttackComponent;
class UCameraComponent;
class UEquipmentComponent;
class UExperienceComponent;
class UCharacterWidgetComponent;
class UHealthComponent;
class UMaterial;
class USkillComponent;
class USkeletalMesh;
class USpringArmComponent;

UCLASS()
class ALZADI_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	// Sets default values for this character's properties

public:
	ABaseCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, Category=AIPlayer)
	TSubclassOf<AAlzadiProxyCharacter> AlzadiProxyCharacterClass;

	UPROPERTY(Replicated)
	AAlzadiProxyCharacter* AlzadiProxyCharacter;

	UPROPERTY(EditDefaultsOnly, Category=AIPlayer)
	TSubclassOf<AAlzadiProxyCharacterAIController> AlzadiProxyCharacterAIControllerClass;

	TObjectPtr<AAlzadiProxyCharacterAIController> AlzadiProxyCharacterAIController;

	UPROPERTY(ReplicatedUsing=OnRep_bIsVisible)
	bool bIsVisible = false;

	void SetbIsVisible(bool InValue);

	UFUNCTION()
	void OnRep_bIsVisible();

	void OnbIsVisibleUpdate();

	UPROPERTY(EditDefaultsOnly, Category=Target)
	bool bAIFollowActivated = false;

	// game mode reference
	TObjectPtr<AAlzadiGameMode> AlzadiGameModeRef;
	void SetAlzadiGameModeRef(AAlzadiGameMode* GameMode);

	// controller references
	TObjectPtr<AAlzadiPlayerController> AlzadiPlayerControllerRef;

	TObjectPtr<AAlzadiAIController> AlzadiAIControllerRef;

	void SetAlzadiPlayerControllerRef(AAlzadiPlayerController* PlayerController);

	void SetAlzadiAIControllerRef(AAlzadiAIController* AIController);

	// type
	ECharacterType CharacterType = ECharacterType::None;
	void SetCharacterType(ECharacterType InCharacterType);
	ECharacterType GetCharacterType();

	// camera
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArmComponent;

	// mesh
	UPROPERTY(ReplicatedUsing=OnRep_Appereance)
	FAppearence Appearence;

	void SetAppereance(FAppearence InAppereance);

	UFUNCTION()
	void OnRep_Appereance();

	void OnAppereanceUpdate();

	// Crossair targeting

	UPROPERTY(EditAnywhere, Category = "Target")
	UStaticMeshComponent* PlaneComponent;

	// Lock target
	UPROPERTY(EditDefaultsOnly, Category=Target)
	float LockedTargetMinimumDistance = 0.f;

	void GetTargeatablePlayers(TArray<ABaseCharacter*>& OutPlayers);

	void GetPointsAroundCharacter(TArray<FVector>& OutPoints, int32 NumberOfPoints,
	                              float DistanceFromCharacter);

	UPROPERTY(Replicated, VisibleAnywhere, Category="Target")
	ABaseCharacter* LockedTarget;

	UFUNCTION(Server, Reliable, WithValidation, Category="Target")
	void SetLockedTargetServer(ABaseCharacter* Character);

	void SetLockedTarget(ABaseCharacter* Character);

	TSet<ABaseCharacter*> LockedBy;

	UFUNCTION(Server, Reliable, WithValidation)
	void ResetLockedTarget();

	void ResetCharacterAsLockedTarget();

	// Experience and Level
	UPROPERTY(VisibleAnywhere, Category="Experience")
	UExperienceComponent* ExperienceComponent;

	// Health
	UPROPERTY(VisibleAnywhere, Category="Health")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category="Health")
	UCharacterWidgetComponent* CharacterWidgetComponent;

	// Death
	void HandleDeath();

	void HideCharacter(bool bHideCharacter);

	// spawn point
	FLocation SpawnPoint;

	void SetSpawnPoint(FLocation Location);
	FLocation GetSpawnPoint();

	// name
	UPROPERTY(Replicated)
	FName CharacterName;

	void SetCharacterName(FName Name);
	FName GetCharacterName();

	// Skills component

	UPROPERTY(VisibleAnywhere, Category="Skills")
	USkillComponent* SkillComponent;

	// Inventory component
	UPROPERTY(VisibleAnywhere, Category="Items")
	UEquipmentComponent* EquipmentComponent;

	// Attack
	UPROPERTY(VisibleAnywhere, Category="Attack")
	UAttackComponent* AttackComponent;

	FORCEINLINE UAttackComponent* GetAttackComponent() { return AttackComponent; }

	// only monster
	UPROPERTY(EditInstanceOnly, Category=Monsters, meta=(RowType=AlzadiCharacter))
	FDataTableRowHandle Monster;

	UPROPERTY(EditDefaultsOnly, Category=Monsters)
	float SpawnRate = 60.f;

	void SpawnCharacter();

	FTimerHandle TimerHandle;

	// Falta ordenar

	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION()
	void HandleDestruction(FData Data);
};
