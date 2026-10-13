// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include <Interface/ABCharacterAIInterface.h>
#include <AIController.h>

UBTTask_Attack::UBTTask_Attack()
{
	// 노드 이름 설정.
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 공격을 수행했다는 메시지를 NPC에 전달.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 메시지 전달을 위해서 인터페이스로 형변환.
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 공격 메시지 전달.
	AIPawn->AttackByAI();

	// 공격 명령을 실행한 후에 곧바로 종료해도 되는지 알 수 없기 때문에
	// InProgress(진행중) 반환.
	return EBTNodeResult::InProgress;
}
