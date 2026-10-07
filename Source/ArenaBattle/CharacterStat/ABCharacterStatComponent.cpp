// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHp = 200.0f;
	SetHp(MaxHp);
}

// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
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
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	// 체력 변경 델리게이트 발행.
	OnHpChanged.Broadcast(CurrentHp);
}
