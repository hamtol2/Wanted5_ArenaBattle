// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <GameData/ABCharacterStat.h>
#include "ABCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	// 스탯을 업데이트할 때 사용할 함수.
	void UpdateStat(
		const FABCharacterStat& InBaseStat,
		const FABCharacterStat& InModifierStat
	);

private:
	// 스탯 위젯이 가진 UI 항목 중에서 Base 스탯에 해당하는 텍스트 블록 모음.
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	// 스탯 위젯이 가진 UI 항목 중에서 Modifier 스탯에 해당하는 텍스트 블록 모음.
	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};
