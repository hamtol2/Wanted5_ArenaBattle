// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ABAIController.h"
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>

AABAIController::AABAIController()
{
	// 애셋 로드 및 설정.
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(
		TEXT("/Game/ArenaBattle/AI/BB_ABCharacter.BB_ABCharacter")
	);

	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(
		TEXT("/Game/ArenaBattle/AI/BT_ABCharacter.BT_ABCharacter")
	);

	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AABAIController::RunAI()
{
	// 블랙보드 컴포넌트 가져오기.
	UBlackboardComponent* BlackboardPtr = Blackboard;

	// 사용할 블랙보드 지정.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 비헤이비어 트리 실행.
		bool RunResult = RunBehaviorTree(BTAsset);

		// 실행 성공여부 확인.
		ensureAlways(RunResult);
	}
}

void AABAIController::StopAI()
{
	// 실행 중인 BehaviorTreeComponent 가져오기.
	UBehaviorTreeComponent* BTComponent 
		= Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		// BT 실행 중지.
		BTComponent->StopTree();
	}
}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AI 실행.
	RunAI();
}
