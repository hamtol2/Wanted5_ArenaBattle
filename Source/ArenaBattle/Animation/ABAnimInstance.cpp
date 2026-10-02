// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

UABAnimInstance::UABAnimInstance()
{
	// 기본 값 설정.
	MovingThreshold = 3.0f;

	// 점프 중인지 판단할 기준 값.
	JumpingThreshold = 100.0f;
}

void UABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 애니메이션을 소유하는 캐릭터 저장.
	Owner = Cast<ACharacter>(GetOwningActor());

	// 캐릭터 무브먼트 컴포넌트 저장.
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 애니메이션 재생에 사용할 값 가져오기.
	if (!Movement)
	{
		return;
	}

	// 현재 속도 저장.
	Velocity = Movement->Velocity;

	// 지면에서 이동하는 속력(빠르기).
	GroundSpeed = Velocity.Size2D();

	// 이동/정지 상태 설정.
	bIsIdle = GroundSpeed < MovingThreshold;

	// 공중에 떠 있는지 확인.
	bIsFalling = Movement->IsFalling();

	// 점프 중인지 판단.
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
}
