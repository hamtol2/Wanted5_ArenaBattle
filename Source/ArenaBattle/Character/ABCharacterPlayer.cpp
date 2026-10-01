// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>

AABCharacterPlayer::AABCharacterPlayer()
{
	// 회전 속성 설정.
	bUseControllerRotationYaw = false;		// Z축 회전.
	bUseControllerRotationPitch = false;	// Y축 회전.
	bUseControllerRotationRoll = false;		// X축 회전.

	// 컴포넌트 생성 및 구성.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(
		TEXT("SpringArm")
	);

	// 계층 설정 (루트 컴포넌트 아래로).
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;

	// 컨트롤러 회전을 사용하도록 설정 (기본 값은 폰의 회전 속성 사용).
	SpringArm->bUsePawnControlRotation = true;

	// 카메라 컴포넌트 생성.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 무브먼트 컴포넌트 설정.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	// 메시 컴포넌트 설정.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	// 메시 애셋 지정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple")
	);

	// 애셋 로드에 성공하면 스켈레탈 메시 설정.
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	// 애님 블루프린트 클래스 검색 및 설정.
	// /Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C")
	);

	// 검색에 성공하면 클래스 정보 설정.
	if (CharacterAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CharacterAnim.Class);
	}
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();


}
