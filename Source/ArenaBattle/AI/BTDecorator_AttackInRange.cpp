// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "ABAI.h"
#include <Interface/ABCharacterAIInterface.h>
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
    // 노드 이름 설정.
    NodeName = TEXT("Can Attack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(
    UBehaviorTreeComponent& OwnerComp, 
    uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    // 감지된 캐릭터와 NPC의 거리가 공격 가능 범위보다 작은지 비교.
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return false;
    }

    IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        return false;
    }

    // 감지된 캐릭터 정보를 블랙보드에서 얻어오기.
    APawn* Target = Cast<APawn>(
        OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET)
    );

    if (nullptr == Target)
    {
        return false;
    }

    // 캐릭터와의 거리 계산.
    float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

    // 공격 범위 가져오기.
    float AttackRange = AIPawn->GetAIAttackRange();

    // 결과 계산.
    bResult = (DistanceToTarget <= AttackRange);

    // 결과 반환.
    return bResult;
}
