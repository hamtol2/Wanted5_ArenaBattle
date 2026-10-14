// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UABHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 캐릭터의 스탯 컴포넌트의 데이터가 HUD가 관리하는 스탯 위젯에 설정되어야 함.
}
