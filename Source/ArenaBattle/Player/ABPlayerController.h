// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

// 전방 선언.
class UABHUDWidget;

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();
	
private:
	virtual void BeginPlay() override;

protected:
	// 위젯 클래스 정보.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UABHUDWidget> ABHUDWidgetClass;

	// 생성한 위젯을 저장할 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<UABHUDWidget> ABHUDWidget;
};
