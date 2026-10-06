// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControlData.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 맵(TMap) 설정.
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(
		TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder")
	);

	if (ShoulderDataRef.Succeeded())
	{
		CharacterControlManager.Add(
			ECharacterControlType::Shoulder,
			ShoulderDataRef.Object
		);
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(
		TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater")
	);

	if (QuaterDataRef.Succeeded())
	{
		CharacterControlManager.Add(
			ECharacterControlType::Quater,
			QuaterDataRef.Object
		);
	}
}

void AABCharacterBase::SetCharacterControlData(
	const UABCharacterControlData* InCharacterControlData)
{
	// 데이터에서 속성을 가져와서 필요한 곳에 설정.

	// Pawn 설정.
	bUseControllerRotationYaw 
		= InCharacterControlData->bUseControllerRotationYaw;

	// 캐릭터 무브먼트 설정.
	GetCharacterMovement()->bUseControllerDesiredRotation
		= InCharacterControlData->bUseControllerDesiredRotation;
	
	GetCharacterMovement()->bOrientRotationToMovement
		= InCharacterControlData->bUseOrientToMovement;

	GetCharacterMovement()->RotationRate
		= InCharacterControlData->RotationRate;
}

void AABCharacterBase::ComboActionEnded(
	UAnimMontage* TargetMontage, bool bInterrupted)
{
	// 몽타주 재생이 종료되면 캐릭터 이동 복구.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
