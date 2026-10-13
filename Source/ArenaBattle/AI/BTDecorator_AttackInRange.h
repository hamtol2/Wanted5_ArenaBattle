// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_AttackInRange();
	
protected:
	// 데코레이터의 조건을 평가할 때 실행되는 함수 오버라이드.
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory) const override;
};
