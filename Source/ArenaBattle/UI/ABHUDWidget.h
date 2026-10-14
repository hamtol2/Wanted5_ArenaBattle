// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameData/ABCharacterStat.h>
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UABHUDWidget(const FObjectInitializer& ObjectInitializer);

	// 스탯이 업데이트 됐을 때 호출될 함수.
	void UpdateStat(
		const FABCharacterStat& InBaseStat,
		const FABCharacterStat& InModifierStat
	);

	// HP 정보가 변경됐을 때 호출될 함수.
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeOnInitialized() override;

protected:
	// HPBar 위젯.
	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBar;

	// 스탯 위젯.
	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStat;
};
