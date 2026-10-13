// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include <Physics/ABCollision.h>
#include <Interface/ABCharacterAIInterface.h>
#include "ABAI.h"

#include <AIController.h>
#include <Engine/OverlapResult.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTService_Detect::UBTService_Detect()
{
	// 에디터에 보이는 노드 이름 값.
	NodeName = TEXT("Detect");

	// 서비스가 실행될 간격(단위: 초).
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 충돌 기능을 활용하기 위해 월드 객체 얻어오기.
	// -> 제어하는 폰으로부터 얻어오기.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return;
	}

	// 충돌 기능을 활용해 캐릭터 감지.
	UWorld* World = ControllingPawn->GetWorld();
	if (!World)
	{
		return;
	}

	// 충돌 범위를 얻어올 인터페이스로 형변환.
	IABCharacterAIInterface* AIPawn
		= Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	// 충돌 결과로 정보를 반환받을 변수.
	TArray<FOverlapResult> OverlapResults;

	// 충돌 감지를 시작할 위치 = 제어하는 폰의 위치.
	FVector Center = ControllingPawn->GetActorLocation();

	// 충돌 감지할 반경.
	float DetectRadius = AIPawn->GetAIDetectRange();

	// 콜리전 파라미터.
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Detect),
		false,
		ControllingPawn
	);

	// 충돌 기능 활용.
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		Params
	);

	// 충돌 감지 됐을 때 처리.
	if (bResult)
	{
		// 감지된 플레이어에 대한 처리 (루프로 진행).
		for (const auto& OverlapResult : OverlapResults)
		{
			// 충돌에서 감지한 액터를 폰으로 형변환.
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// 플레이어인지 확인 => 폰을 제어하는 컨트롤러가 플레이어 컨트롤러인지 확인.
			if (nullptr != Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 감지된 정보를 블랙보드에 저장.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(
					BBKEY_TARGET,
					Pawn
				);

				// 디버그 드로우를 활용해서 시각적으로 표시.
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 감지된 지점 표시.
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);

				// 감지된 방향을 선으로 표시.
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.3f);

				return;
			}
		}
	}

	// 충돌 감지를 못한 경우에는 타겟 액터를 null로 지정.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	// 감지 영역을 빨간색으로 표시.
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
