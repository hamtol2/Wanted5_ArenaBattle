// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include <GameData/ABCharacterStat.h>
#include "ABPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABPotionItemData : public UABItemData
{
	GENERATED_BODY()
	
public:
	UABPotionItemData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	// HP 회복량.
	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;
};
