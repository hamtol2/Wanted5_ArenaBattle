// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include <GameData/ABGameSingleton.h>

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//SetHp(MaxHp);

	// bWantsInitializeComponent 값을 true로 설정해야 
	// InitializeComponent 함수가 호출됨.
	bWantsInitializeComponent = true;

	// 현재 레벨 설정.
	CurrentLevel = 1;

	// 공격 반경 기본값 설정.
	AttackRadius = 50.0f;
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// 현재 레벨 설정. 설정할 수 있는 최대 레벨을 벗어나지 않도록 방지.
	CurrentLevel = FMath::Clamp(
		InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel
	);

	// 게임 싱글톤에 접근해서 레벨에 해당하는 스탯 데이터 설정.
	BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);

	// 값에 문제가 없는지 확인.
	ensureAlways(BaseStat.MaxHp > 0.0f);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// 기존 HP 값 임시 저장.
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	// 대미지를 적용한 새 HP 계산.
	SetHp(PrevHp - ActualDamage);

	// HP가 모두 소멸(0) 되었는지 확인.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// 델리게이트 발행.
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	// 현재 체력 업데이트.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	// 체력 변경 델리게이트 발행.
	OnHpChanged.Broadcast(CurrentHp);
}
