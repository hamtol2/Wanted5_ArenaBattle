// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>

#include <InputMappingContext.h>
#include <InputAction.h>

#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

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

	// 입력 관련 애셋 로드 및 설정.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(
		TEXT("/Game/ArenaBattle/Input/IMC_Default.IMC_Default")
	);

	if (DefaultMappingContextRef.Succeeded())
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move")
	);

	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look")
	);

	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump")
	);

	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 사용할 입력 매핑 컨텍스트 설정.
	// 플레이어 컨트롤러 가져오기.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		// 향상된 입력 서브 시스템 가져오기.
		UEnhancedInputLocalPlayerSubsystem* InputSystem 
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()
		);

		if (InputSystem)
		{
			InputSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AABCharacterPlayer::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 바인딩 - 입력 액션을 통해서 입력이 전달될 때 실행함 함수 연동.
	// 향상된 입력 컴포넌트로 변환.
	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AABCharacterPlayer::Move
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AABCharacterPlayer::Look
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&ACharacter::Jump
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&ACharacter::StopJumping
		);
	}
}

void AABCharacterPlayer::Move(const FInputActionValue& Value)
{
	// 입력 값 읽어오기 ( 입력에 지정된 타입으로 변환 ).
	FVector2D Movement = Value.Get<FVector2D>();

	// 이동할 방향 만들기.
	// 카메라가 바라보는 방향(=컨트롤러가 바라보는 방향)을 기준으로 방향 만들기.

	// 방향을 구하기 위해서는 회전(오리엔테이션)을 먼저 구해야 함.
	// FRotator는 오일러(Euler) 회전을 표기하는데 사용됨.
	// 오일러 회전은 X축으로 몇도, Y축으로 몇도, Z축으로 몇도를 직관적으로 표기.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 앞방향.
	// 회전 행렬을 구하고, 거기에서 앞방향 성분을 추출.
	FVector ForwardVector 
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// 오른쪽 방향.
	FVector RightVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트 컴포넌트에 입력 전달.
	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);
}

void AABCharacterPlayer::Look(const FInputActionValue & Value)
{
	// 입력 값 읽어오기 ( 입력에 지정된 타입으로 변환 ).
	FVector2D RotationValue = Value.Get<FVector2D>();

	// 회전 처리 -> 컨트롤러에 회전 입력 전달.
	AddControllerYawInput(RotationValue.X);
	AddControllerPitchInput(RotationValue.Y);
}
