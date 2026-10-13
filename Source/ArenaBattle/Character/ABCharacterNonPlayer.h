// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include <Interface/ABCharacterAIInterface.h>
#include <Engine/StreamableManager.h>
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(Config = ArenaBattle)
class ARENABATTLE_API AABCharacterNonPlayer 
	: public AABCharacterBase,
	public IABCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

protected:
	// 랜덤으로 메시 로드할 때 사용할 초기화 함수.
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetDead() override;

	// NPC 메시의 로드가 완료됐을 때 콜백을 받을 함수.
	void NPCMeshLoadCompleted();

	// config 파일에 설정된 경로 값을 배열로 관리.
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	// 배열에 채워진 경로 값을 활용해 애셋을 비동기로 로드할 때 사용할 핸들.
	TSharedPtr<FStreamableHandle> NPCMeshHandle;

	// AI 섹션.
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual void AttackByAI() override;
};
