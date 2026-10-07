// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ABWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	// 위젯 생성할 때 호출되는 함수.
	// 이 함수를 오버라이드하면 위젯이 생성됐다는 걸 보장받음.
	virtual void InitWidget() override;
};
