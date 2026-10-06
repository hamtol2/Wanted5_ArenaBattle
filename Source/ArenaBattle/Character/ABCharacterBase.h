// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

// 입력 컨트롤을 관리하기 위한 열거형.
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

protected:

	// 컨트롤 데이터 설정.
	virtual void SetCharacterControlData(
		const class UABCharacterControlData* InCharacterControlData
	);

	// 콤보 공격 처리 함수.
	// 공격을 처음 시작할 때와 콤보 액션을 진행할 때 실행.
	void ProcessComboCommand();

	// 콤보 공격이 시작될 때 실행할 함수.
	void ComboActionBegin();

	// 몽타주 재생 종료 시 호출할 함수 (델리게이트와 연동).
	void ComboActionEnded(UAnimMontage* TargetMontage, bool bInterrupted);

	// 콤보 타이머 설정 함수.
	void SetComboCheckTimer();

	// 콤보 타이밍 처리 함수.
	// 설정된 시간 이전에 입력이 제대로 들어왔는지 확인하는데 사용.
	void ComboCheck();

protected:
	// 컨트롤 타입별로 컨트롤 데이터를 관리하기 위한 맵.
	// TMap -> 키-값 쌍으로 저장하는 자료구조.
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

	// 콤보 몽타주 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	// 콤보 액션 처리 데이터.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UABComboActionData> ComboActionData;

	// 현재 재생중인 콤보 단계 추적 변수(0은 실행 안됨. 1/2/3/4 단계로 구분).
	UPROPERTY(VisibleAnywhere, Category = Attack)
	uint32 CurrentCombo = 0;

	// 콤보 판정에 사용할 타이머 핸들.
	FTimerHandle ComboTimerHandle;

	// 콤보 점프(섹션 점프) 판정할 때 사용할 플래그.
	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bHasNextComboCommand = false;
};
