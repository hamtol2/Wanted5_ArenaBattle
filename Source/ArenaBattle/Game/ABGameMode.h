// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Interface/ABGameInterface.h>
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameMode 
	: public AGameModeBase,
	public IABGameInterface
{
	GENERATED_BODY()
	
public:
	// 생성자.
	AABGameMode();

protected:
	// Inherited via IABGameInterface
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

public:
	// 게임을 클리어하기 위한 조건 (점수).
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	// 현재 게임에서 획득한 점수.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	// 게임 클리어 여부.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;


};
