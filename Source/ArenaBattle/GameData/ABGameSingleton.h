// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

// 로그 카테고리 추가.
DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UABGameSingleton();

	// 전역 접근 함수.
	static UABGameSingleton& Get();

	// 레벨을 전달해서 스탯 데이터를 반환하는 함수.
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const
	{
		// 레벨을 사용해서 배열에 접근할 인덱스 계산.
		int32 StatIndex = InLevel - 1;

		// 인덱스가 유효한지 여부에 따른 데이터 반환.
		return CharacterStatTable.IsValidIndex(StatIndex) ?
			CharacterStatTable[StatIndex] : FABCharacterStat();
	}

	// 최대 레벨.
	UPROPERTY()
	int32 CharacterMaxLevel;
	
private:
	// 스탯 데이터 테이블 관리.
	TArray<FABCharacterStat> CharacterStatTable;
};
