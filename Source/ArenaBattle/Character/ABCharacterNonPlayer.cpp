// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 5초 후에 액터 제거.
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				// 액터 제거.
				Destroy();
			}
		),
		DeadEventDelayTime,
		false
	);
}
