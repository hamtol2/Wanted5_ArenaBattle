// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, 
    uint8* NodeMemory)
{
    // 상위 로직 호출.
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    // 다음에 정찰할 위치 선택 로직.

    // 네비게이션 기능 활용.

    return EBTNodeResult::Type();
}
