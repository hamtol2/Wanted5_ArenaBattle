// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "ABHpBarWidget.h"
#include "ABCharacterStatWidget.h"
#include <Interface/ABCharacterHUDInterface.h>

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UABHUDWidget::UpdateStat(
	const FABCharacterStat& InBaseStat, 
	const FABCharacterStat& InModifierStat)
{
	// HpBar에 최대 체력 스탯 업데이트.
	FABCharacterStat TotalStat = InBaseStat + InModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp);

	// 캐릭터 스탯 위젯에 새로운 데이터 전달.
	CharacterStat->UpdateStat(InBaseStat, InModifierStat);
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	// Hp 상태 업데이트.
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UABHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// HUD에서 이름으로 위젯 검색.
	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));

	// 제대로 로드 됐는지 확인.
	ensureAlways(nullptr != HpBar);

	CharacterStat = Cast<UABCharacterStatWidget>(
		GetWidgetFromName(TEXT("WidgetCharacterStat"))
	);

	// 제대로 로드 됐는지 확인.
	ensureAlways(nullptr != CharacterStat);

	// 캐릭터의 스탯 컴포넌트의 데이터가 HUD가 관리하는 스탯 위젯에 설정되어야 함.
	//GetOwningPlayer()->GetPawn();
	IABCharacterHUDInterface* HUDPawn 
		= GetOwningPlayerPawn<IABCharacterHUDInterface>();
	if (nullptr != HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
