// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttackHitCheck.generated.h"

/**
 *
 */
UCLASS()
class ARENABATTLE_API UAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

protected:
	// 애니메이션 노타파이가 실행될 때 호출되는 함수 오버라이드.
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};
