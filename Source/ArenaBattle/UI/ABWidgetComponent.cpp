// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 앞선 단계에서 위젯 컴포넌트에서 생성한 위젯 객체를 원하는 타입으로 형변환.
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		// 위젯 컴포넌트를 소유하는 액터 정보를 위젯에 전달.
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
