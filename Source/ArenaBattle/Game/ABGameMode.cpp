// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include <Player/ABPlayerController.h>
#include <Character/ABCharacterPlayer.h>

AABGameMode::AABGameMode()
{
	// 게임에서 사용할 클래스 타입 선언.
	DefaultPawnClass = AABCharacterPlayer::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();

	// 기본 값 설정.
	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AABGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	// 현재 점수를 전달받은 값으로 업데이트.
	CurrentScore = NewPlayerScore;

	// 게임 클리어 여부 확인.
	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;
	}

	// 플레이어 컨트롤러에게 플레이어 점수가 업데이트 되었다는 걸 알리기.
	// 기본적으로 플레이어 컨트롤러는 "플레이어"의 수 만큼 생성됨.
	// 하지만 싱글 플레이어 게임의 경우 "플레이어(게이머)"가 1개임.
	AABPlayerController* ABPlayerController 
		= Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr != ABPlayerController)
	{
		// 게임 점수가 변경된 사실을 알리기.
		ABPlayerController->GameScoreChanged(CurrentScore);

		// 게임 클리어 여부 확인.
		if (bIsCleared)
		{
			ABPlayerController->GameClear();
		}
	}
}

void AABGameMode::OnPlayerDead()
{
	AABPlayerController* ABPlayerController
		= Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr != ABPlayerController)
	{
		// 플레이어가 죽었으면 게임 오버 처리.
		ABPlayerController->GameOver();
	}
}

bool AABGameMode::IsGameCleared()
{
	return bIsCleared;
}

int32 AABGameMode::GetClearScore()
{
	return ClearScore;
}
