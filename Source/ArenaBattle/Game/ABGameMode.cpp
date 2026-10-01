// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
//#include <Player/ABPlayerController.h>
#include <Character/ABCharacterPlayer.h>

AABGameMode::AABGameMode()
{
	// 게임에서 사용할 클래스 타입 선언.

	/*
	* /Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter
	*/

	// 블루프린트로부터 클래스 정보 로드.
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClssRef(
	//	TEXT("/Game/ThirdPerson/Blueprints///BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C")
	//);
	//
	//// 로드 성공한 경우 클래스 지정.
	//if (ThirdPersonClssRef.Succeeded())
	//{
	//	DefaultPawnClass = ThirdPersonClssRef.Class;
	//}
	
	DefaultPawnClass = AABCharacterPlayer::StaticClass();

	// 플레이어 컨트롤러 클래스 설정.
	//PlayerControllerClass = AABPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonPlayerController.BP_ThirdPersonPlayerController_C")
	);

	if (PlayerControllerClassRef.Succeeded())
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
