// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

// 로그 카테고리 정의.
DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	// 스탯 데이터 테이블 애셋을 읽어서 데이터 배열로 구성.

	// 데이터 테이블 애셋 로드.
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
		TEXT("/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable")
	);

	// 로드 성공하면 이어서 처리.
	if (DataTableRef.Succeeded())
	{
		// UDataTable은 데이터를 내부에서 TMap<FName, uint8*>로 관리함.
		const UDataTable* DataTable = DataTableRef.Object;

		// 로드한 데이터 확인.
		ensureAlways(DataTable->GetRowMap().Num() > 0);

		// 전체 Row(행) 데이터 가져오기.
		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		// 타입 형변환.
		for (uint8* Value : ValueArray)
		{
			CharacterStatTable.Add( *reinterpret_cast<FABCharacterStat*>(Value) );
		}

		// 스탯 테이블의 원소(데이터) 개수를 최대 레벨로 설정.
		CharacterMaxLevel = CharacterStatTable.Num();

		// 값 확인.
		ensureAlways(CharacterStatTable.Num() > 0);
	}
}

UABGameSingleton& UABGameSingleton::Get()
{
	// 언리얼을 통해서 인스턴스(싱글톤 객체) 값 가져오기.
	UABGameSingleton* Instance
		= CastChecked<UABGameSingleton>(GEngine->GameSingleton);

	if (Instance)
	{
		return *Instance;
	}

	// 오류 상황. 여기에 접근되면 안됨.
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));

	// 반환이 필요하기 때문에 추가하는 코드. 여기에 접근되면 안됨.
	return *NewObject<UABGameSingleton>();
}
