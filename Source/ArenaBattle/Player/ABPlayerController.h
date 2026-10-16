// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

// 전방 선언.
class UABHUDWidget;

// 로그 카테고리 추가.
DECLARE_LOG_CATEGORY_EXTERN(LogABPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

	// 게임 처리와 관련된 함수.
	// 공개 메소드 - 객체 지향(메시지).
	// 객체 지향 설계에서 객체 간의 소통은 "메시지"를 통해서.
	// 메소드(방법) - 메시지로 요청된 작업을 실제로 처리하는 방법(로직).
	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();

public:
	// 블루프린트에서 구현 가능한 특별한 함수로 선언.
	// K2 접두어는 Kismet -> 블루프린트의 예전 버전.
	// BlueprintImplementableEvent는 구현 배치 안함.
	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	// 게임 재시작 횟수가 변경되면 사용할 이벤트.
	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount);
	
private:
	virtual void BeginPlay() override;

protected:
	// 위젯 클래스 정보.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UABHUDWidget> ABHUDWidgetClass;

	// 생성한 위젯을 저장할 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<UABHUDWidget> ABHUDWidget;

	// SaveGame 섹션.
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UABSaveGame> SaveGameInstance;
};
