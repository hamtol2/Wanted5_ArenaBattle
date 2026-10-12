#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "ABCharacterStat.generated.h"


USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterStat()
		: MaxHp(0.0f), 
		Attack(0.0f), 
		AttackRange(0.0f), 
		AttackSpeed(0.0f), 
		MovementSpeed(0.0f)
	{
	}

	// 덧셈 연산자 오버로딩.
	// 모든 속성이 float 타입의 데이터.
	// 가상 함수를 사용하지 않는 구조체 타입.
	// -> 가상 함수를 활용하면 vtable이 추가되고 이렇게 되면 타입의 크기를 확신할 수 없음.
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		// 두 데이터를 더해야하는 상황.
		// -> 현재 데이터 + 다른 데이터.

		// 현재 데이터의 포인터.
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		
		// 덧셈 처리할 다른 데이터의 포인터.
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		// 결과를 저장할 구조체 변수.
		FABCharacterStat Result;

		// 결과 구조체 포인터.
		float* ResultPtr = reinterpret_cast<float*>(&Result);

		// 구조체에 포함된 float 변수의 개수.
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);

		// 루프 순회를 통한 덧셈 처리.
		for (int32 ix = 0; ix < StatNum; ++ix)
		{
			ResultPtr[ix] = ThisPtr[ix] + OtherPtr[ix];
		}

		// 결과 반환.
		return Result;
	}

public:
	// 스탯에서 사용할 속성.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
};