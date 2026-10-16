// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include <UI/ABHUDWidget.h>

AABPlayerController::AABPlayerController()
{
	// 위젯 블루프린트 애셋에서 클래스 정보 바인딩(연결).
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(
		TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C")
	);

	// 로드에 성공하면 클래스 설정.
	if (ABHUDWidgetRef.Succeeded())
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::GameScoreChanged(int32 NewScore)
{
	// 블루프린트에서 구현할 함수가 호출될 수 있도록 처리.
	K2_OnScoreChanged(NewScore);
}

void AABPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AABPlayerController::GameOver()
{
	K2_OnGameOver();
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 설정.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 위젯 생성.
	//ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	//
	//// 문제 없이 생성됐다면 화면에 추가.
	//if (nullptr != ABHUDWidget)
	//{
	//	ABHUDWidget->AddToViewport();
	//}
}
