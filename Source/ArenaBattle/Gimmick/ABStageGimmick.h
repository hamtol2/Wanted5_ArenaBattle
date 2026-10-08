// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABStageGimmick();

	// 스테이지 관련 변수 및 함수.
protected:
	// 스테이지 메시를 보여줄 때 사용할 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;

	// 스테이지 진입 여부를 판단할 때 사용할 트리거.
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	// 스테이지 트리거에 콜백으로 등록할 함수 선언.
	UFUNCTION()
	void OnStageTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 문(게이트) 관련 변수 및 함수 선언.
protected:
	// 4개의 문을 저장하는데 맵 컨테이너 활용. 이름 값으로 문을 관리.
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	// 문 트리거를 배열로 관리.
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	// 문에 배치한 박스 컴포넌트와 충돌했을 때 실행할 콜백 함수.
	UFUNCTION()
	void OnGateTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
