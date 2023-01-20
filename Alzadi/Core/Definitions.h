#pragma once
#include "Engine/DataTable.h"

#include "Definitions.generated.h"

class ABaseCharacter;

// common definitions

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None,
	Melee,
	Distance,
	Magic
};

UENUM()
enum class ECharacterType : uint8
{
	None,
	Player,
	NPC,
	Monster
};

USTRUCT()
struct FLocation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float X = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Y = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Z = 0.f;

	FVector ToFVector()
	{
		return FVector(this->X, this->Y, this->Z);
	}

	void FromFVector(FVector InLocation)
	{
		this->X = InLocation.X;
		this->Y = InLocation.Y;
		this->Z = InLocation.Z;
	}
};

USTRUCT()
struct FSkill
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Melee = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Distance = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Magic = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Defense = 0.f;
};

// web server definitions

USTRUCT()
struct FItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(GetOptions="GetAllItemOptions"))
	FName Id;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMax=100))
	float Quantity = 0.f;
};


USTRUCT()
struct FEquipment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FItem> Set;

	UPROPERTY(EditDefaultsOnly)
	TArray<FItem> Inventory;
};

USTRUCT(BlueprintType)
struct FAttack
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	EAttackType Type = EAttackType::None;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	float Range = 0.f;
};

USTRUCT(BlueprintType)
struct FAppearence
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimBlueprint* AnimBlueprint = nullptr;
};

USTRUCT(BlueprintType)
struct FAlzadiCharacter : public FTableRowBase
{
	GENERATED_BODY()

	// players, npc and monsters

	// not in component
	UPROPERTY()
	ECharacterType CharacterType = ECharacterType::None;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType != ECharacterType::Player", EditConditionHides))
	FName Name;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType == ECharacterType::Player", EditConditionHides))
	FLocation SpawnPoint;

	UPROPERTY()
	FLocation Location;

	// health component
	UPROPERTY()
	float Health = 0.f;

	// skill component
	UPROPERTY(EditDefaultsOnly)
	FSkill Skill;

	// only players

	// experience component
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType == ECharacterType::Player", EditConditionHides))
	float Experience = 0.f;

	// equipment component
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType == ECharacterType::Player", EditConditionHides))
	FEquipment Equipment;

	// only monsters

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType != ECharacterType::Player", EditConditionHides))
	FAppearence Appearence;

	// attack component
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType != ECharacterType::Player", EditConditionHides))
	FAttack Weapon;

	// experience component
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType != ECharacterType::Player", EditConditionHides))
	float Level = 0.f;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="CharacterType != ECharacterType::Player", EditConditionHides))
	float KillExperience = 0.f;
};

USTRUCT()
struct FAlzadiAccount
{
	GENERATED_BODY()

	UPROPERTY()
	FString Account;

	UPROPERTY()
	FString Password;
};

USTRUCT()
struct FContent
{
	GENERATED_BODY()

	UPROPERTY()
	FAlzadiCharacter AlzadiCharacter;

	UPROPERTY()
	FAlzadiAccount AlzadiAccount;
};

USTRUCT()
struct FData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Topic;

	UPROPERTY()
	FString Message;

	UPROPERTY()
	FContent Content;
};


// game server definitions

UENUM(BlueprintType)
enum class EEquipment : uint8
{
	Helmet,
	Weapon,
	Armor,
	Shield,
	Legs,
	Boots,
};

// items

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Helmet,
	Weapon,
	Armor,
	Shield,
	Legs,
	Boots,
	Consumable,
	None
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	// all items

	UPROPERTY(EditDefaultsOnly, Category=Items)
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditDefaultsOnly, Category=Items)
	FName Name;

	UPROPERTY(EditDefaultsOnly, Category=Items)
	FName Description;

	UPROPERTY(EditDefaultsOnly, Category=Items)
	bool bIsStackable = false;

	UPROPERTY(EditDefaultsOnly, Category=Items)
	UTexture2D* Icon = nullptr;

	// weapons

	UPROPERTY(EditDefaultsOnly, Category=Weapon,
		meta=(EditCondition="ItemType == EItemType::Weapon", EditConditionHides))
	FAttack Weapon;
};

USTRUCT()
struct FDamageMultiplier
{
	GENERATED_BODY()

	UPROPERTY()
	float Level = 0.f;

	UPROPERTY()
	float Melee = 0.f;

	UPROPERTY()
	float Distance = 0.f;

	UPROPERTY()
	float Magic = 0.f;
};

USTRUCT(BlueprintType)
struct FExpLevel : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Level = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float Experience = 0.f;
};
