// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include <Components/ProgressBar.h>
#include <Interface/ABCharacterWidgetInterface.h>

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 최대 체력은 외부에서 설정이 되어야함.
	// 따라서 초기값은 오류의 의미를 가진 음수로 설정.
	MaxHp = -1.0f;
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	// 검증.
	ensure(MaxHp > 0.0f);

	// 프로그레스 바 위젯 참조 유효성 확인.
	if (HpProgressBar)
	{
		// 최대 체력 대비 현재 체력의 게이지(퍼센트) 설정.
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이 함수가 호출될 때는 UI(UMG)가 초기화됐다고 생각할 수 있음.

	// 이름 값을 사용해 위젯 참조 가져오기.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	// 인터페이스를 통해서 이 위젯의 함수에 Stat 컴포넌트의 델리게이트 등록 요청.
	IABCharacterWidgetInterface* CharacterWidgetInterface 
		= Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidgetInterface)
	{
		CharacterWidgetInterface->SetupCharacterWidget(this);
	}
}
