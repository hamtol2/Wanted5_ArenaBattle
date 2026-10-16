// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ABSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UABSaveGame();

	// 재시도 횟수를 기록하는데 사용할 변수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RetryCount;
};
