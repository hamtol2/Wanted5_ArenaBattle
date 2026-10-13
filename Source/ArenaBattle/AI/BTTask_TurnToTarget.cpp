// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "ABAI.h"
#include <Interface/ABCharacterAIInterface.h>
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// 노드 이름 설정.
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 캐릭터를 바라보는 방향으로 회전하기.
	// 필요한것: 캐릭터, 캐릭터를 바라보는 방향 구하기, NPC 폰.
	// 회전 처리 로직(한번에 할지/나눠서 할지).

	// 제어하는 폰.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드에 저장된 캐릭터.
	APawn* TargetPawn = Cast<APawn>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET)
	);

	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 부드럽게 회전 처리할 때 사용할 회전 속도 값 얻어오기.
	// -> 인터페이스를 통해서 얻어오기.
	IABCharacterAIInterface* AIPawn
		= Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 회전 속도 값 가져오기.
	float TurnSpeed = AIPawn->GetAITurnSpeed();

	// 바라봐야할 방향 구하기.
	FVector LookVector 
		= TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// 높이 성분은 제거 -> Yaw(Z축) 회전은 XY 평면 항목에만 영향을 받기 때문.
	LookVector.Z = 0.0f;

	// 방향을 기반으로 Rotator 구하기.
	// 1. 회전 행렬 만들기 -> 2.Rotator로 변환.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// 월드 객체 - 델타 타임을 얻어오기 위해.
	UWorld* World = ControllingPawn->GetWorld();

	// 부드럽게 회전 설정.
	// 시작: 현재 회전 값 | 목표: 바라봐야하는 방향을 보는 회전.
	FRotator CurrentRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		TargetRot,
		World->GetDeltaSeconds(),
		TurnSpeed
	);

	// 회전 적용.
	ControllingPawn->SetActorRotation(CurrentRot);

	// 종료(성공 반환).
	return EBTNodeResult::Succeeded;
}
