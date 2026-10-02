// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include <InputActionValue.h>
#include "ABCharacterPlayer.generated.h"

// 전방 선언.
class UInputAction;

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

protected:
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	// 설정된 컨트롤에 따라서 입력 매핑 컨텍스트 및 관련 설정을 처리하는 함수.
	void SetCharacterControl(
		ECharacterControlType NewCharacterControlType
	);

	// 컨트롤 데이터 설정 함수.
	virtual void SetCharacterControlData(
		const class UABCharacterControlData* InCharacterControlData
	) override;

protected:
	// 이동 처리 담당 함수.
	void ShoulderMove(const FInputActionValue& Value);

	// 회전 처리 담당 함수.
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	// V키에 대응해서 실행할 함수.
	void ChangeCharacterControl();

protected:
	// 컴포넌트 구성.
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	// 입력 관련 설정.
protected:
	// 입력 매핑 컨텍스트.
	//UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	//TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// 입력 액션.
	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> ShoulderMoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> ShoulderLookAction;

	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuaterMoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> ChangeControlAction;

	// 현재 사용 중인 컨트롤 타입을 추적(저장)하는 변수.
	UPROPERTY(VisibleAnywhere, Category = CharacterControl)
	ECharacterControlType CurrentCharacterControlType;
};
