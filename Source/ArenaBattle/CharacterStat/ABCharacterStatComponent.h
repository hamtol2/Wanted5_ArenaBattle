// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GameData/ABCharacterStat.h>
#include "ABCharacterStatComponent.generated.h"

// 델리게이트 선언.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

// 스탯 정보가 변경됐을 때 발행할 델리게이트 선언.
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnStatChangedDelegate, 
	const FABCharacterStat& /*InBaseStat*/, 
	const FABCharacterStat& /*InModifierStat*/
);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// Getter.
	//FORCEINLINE float GetMaxHp() const { return MaxHp; }

	// 레벨을 설정하고 기본 스탯 정보를 업데이트하는 함수.
	void SetLevelStat(int32 InNewLevel);

	// 현재 레벨 반환 함수.
	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }

	// 기본 스탯 데이터 설정 함수.
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;

		// 스탯 정보 변경 델리게이트 호출.
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	// 기본 스탯을 더해주는 처리 함수.
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InAddBaseStat)
	{
		BaseStat = BaseStat + InAddBaseStat;

		// 스탯 변경 이벤트 발행.
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	// 부가 스탯 데이터 설정 함수.
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;

		// 스탯 정보 변경 델리게이트 호출.
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }

	// 기본+부가 스탯을 합친 전체 스탯 데이터 반환 함수.
	FORCEINLINE FABCharacterStat GetTotalStat() const
	{
		return BaseStat + ModifierStat;
	}

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	// HP를 회복시킬 때 사용할 함수.
	FORCEINLINE void HealHp(float InHealAmount)
	{
		// 현재 HP에 회복량만큼 더해주기.
		CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp);

		// 델리게이트 호출 (HP 변경 이벤트).
		OnHpChanged.Broadcast(CurrentHp);
	}

	// 공격 반경 반환.
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// 대미지 적용 함수(Setter).
	float ApplyDamage(float InDamage);

protected:
	// HP가 변경됐을 때 실행할 함수.
	void SetHp(float NewHp);

public:
	// 체력을 모두 소진했을 때 발행할 델리게이트 선언.
	FOnHpZeroDelegate OnHpZero;

	// 체력이 변경될 때 발행할 델리게이트 선언.
	FOnHpChangedDelegate OnHpChanged;

	// 스탯 정보가 변경될 때 발행할 델리게이트.
	FOnStatChangedDelegate OnStatChanged;

protected:
	// 기존에 임시로 사용하던 속성 제거.
	// 최대 체력 값.
	//UPROPERTY(VisibleInstanceOnly, Category = Stat)
	//float MaxHp;

	// 현재 체력 값.
	// 런타임에만 사용되고 폐기되는 데이터는 명시적으로 임시(Transient) 키워드를 적용해줄 수 있음.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 캐릭터 레벨.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;

	// 공격 반경.
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float AttackRadius;

	// 캐릭터의 기본 스탯 데이터.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FABCharacterStat BaseStat;

	// 캐릭터의 부가 스탯 데이터.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FABCharacterStat ModifierStat;
};
