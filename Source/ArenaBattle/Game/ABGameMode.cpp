// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include <Player/ABPlayerController.h>
#include <Character/ABCharacterPlayer.h>

AABGameMode::AABGameMode()
{
	// 게임에서 사용할 클래스 타입 선언.
	DefaultPawnClass = AABCharacterPlayer::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
}
