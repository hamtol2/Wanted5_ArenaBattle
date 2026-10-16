// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "ABSaveGame.h"
#include <UI/ABHUDWidget.h>
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(LogABPlayerController);

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

	// 게임이 끝나면 세이브 게임 객체를 활용해 데이터 저장.
	if (false == UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error!!"));
	}

	// 게임이 종료되면 재시도 횟수가 증가하고 이를 알 수 있게 이벤트 실행.
	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 설정.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 게임이 시작되면 저장된 게임이 있는지 확인 및 로드.
	SaveGameInstance 
		= Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));

	// 이미 저장된 게임이 있으면 객체가 null이 아님.
	if (nullptr != SaveGameInstance)
	{
		// 재시도 횟수 증가 처리.
		++SaveGameInstance->RetryCount;
	}

	else
	{
		// 저장된 게임이 없으면 새로 생성.
		SaveGameInstance = NewObject<UABSaveGame>();
	}

	// 게임을 시작할 때 재시도 횟수 정보를 알 수 있게 호출.
	K2_OnGameRetryCount(SaveGameInstance->RetryCount);

	// 위젯 생성.
	//ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);
	//
	//// 문제 없이 생성됐다면 화면에 추가.
	//if (nullptr != ABHUDWidget)
	//{
	//	ABHUDWidget->AddToViewport();
	//}
}
