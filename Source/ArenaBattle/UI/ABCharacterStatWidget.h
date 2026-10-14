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
	
public:
	// 스탯을 업데이트할 때 사용할 함수.
	void UpdateStat(
		const FABCharacterStat& InBaseStat,
		const FABCharacterStat& InModifierStat
	);
};
