// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include <Interface/ABAnimationAttackInterface.h>

void UAnimNotify_AttackHitCheck::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 캐릭터에 접근해서 공격 판정 함수 호출.
	// 직접 캐릭터에 접근하는 대신 인터페이스를 통한 간접 접근.
	// -> 의존성을 줄이기 위해.
	if (MeshComp)
	{
		// 컴포넌트의 소유자(액터)를 원하는 타입(인터페이스)으로 형변환.
		IABAnimationAttackInterface* AttackPawn
			= Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());

		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
