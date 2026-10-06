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

	// 몽타주 재생 종료 시 호출할 함수 (델리게이트와 연동).
	void ComboActionEnded(UAnimMontage* TargetMontage, bool bInterrupted);

protected:
	// 컨트롤 타입별로 컨트롤 데이터를 관리하기 위한 맵.
	// TMap -> 키-값 쌍으로 저장하는 자료구조.
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

	// 콤보 몽타주 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;
};
