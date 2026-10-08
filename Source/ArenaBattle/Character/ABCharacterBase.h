// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Interface/ABAnimationAttackInterface.h>
#include <Interface/ABCharacterWidgetInterface.h>
#include <Interface/ABCharacterItemInterface.h>
#include "ABCharacterBase.generated.h"

// 로그 카테고리 추가.
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

// 아이템 획득 처리에 사용할 델리게이트 선언.
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);

// 입력 컨트롤을 관리하기 위한 열거형.
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class ARENABATTLE_API AABCharacterBase 
	: public ACharacter, 
	public IABAnimationAttackInterface,
	public IABCharacterWidgetInterface,
	public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

protected:

	// 컴포넌트 초기화가 끝났을 때 호출되는 함수
	// -> 즉, 액터의 초기화가 끝난 시점.
	virtual void PostInitializeComponents() override;

	// 액터가 대미지를 받았을 때 호출되는 함수 (Actor에서 파생됨).
	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, 
		AActor* DamageCauser) override;

	// 아이템을 수집했을 때 호출될 함수.
	virtual void TakeItem(class UABItemData* InItemData) override;

	// 아이템 종류를 구분해서 수집을 처리할 함수.
	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

	// Dead 처리.
protected:
	// 죽음 설정 함수.
	virtual void SetDead();

	// 죽는 애니메이션 재생 함수.
	void PlayDeadAnimation();

protected:

	// 컨트롤 데이터 설정.
	virtual void SetCharacterControlData(
		const class UABCharacterControlData* InCharacterControlData
	);

	// 위젯을 설정할 때 사용할 함수.
	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;

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

	// 공격 감지(판정) 함수.
	virtual void AttackHitCheck() override;

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

	// 죽음 애니메이션 몽타주 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<class UAnimMontage> DeadMontage;

	// 죽은 뒤에 약간의 시간을 대기(딜레이)한 후 삭제.
	float DeadEventDelayTime = 5.0f;

protected:
	// 스탯 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

	// 위젯 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;

	// 무기 아이템 획득 시 사용할 스켈레탈 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// 아이템 수집처리에 사용할 델리게이트 배열.
	TArray<FOnTakeItemDelegate> TakeItemActions;
};
