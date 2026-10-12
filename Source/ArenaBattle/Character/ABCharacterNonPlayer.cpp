// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include <Engine/AssetManager.h>

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// 시작할 때는 안보이도록 설정.
	GetMesh()->SetHiddenInGame(true);

	// 메시 컴포넌트 설정.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	// 애님 블루프린트 클래스 검색 및 설정.
	// /Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(
		TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C")
	);

	// 검색에 성공하면 클래스 정보 설정.
	if (CharacterAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CharacterAnim.Class);
	}
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// NPCMeshes 배열이 제대로 설정됐는지 확인.
	ensureAlways(NPCMeshes.Num() > 0);

	// 랜덤 인덱스 선택.
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);

	// 비동기로 메시 로드.
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		NPCMeshes[RandIndex],
		FStreamableDelegate::CreateUObject(
			this,
			&AABCharacterNonPlayer::NPCMeshLoadCompleted
		)
	);
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

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	// 로드가 완료된 핸들이 유효한지 확인.
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());

		// 스켈레탈 메시 애셋 설정.
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);

			// 메시 컴포넌트 보이도록 설정.
			GetMesh()->SetHiddenInGame(false);
		}
	}

	// 사용한 핸들 해제.
	NPCMeshHandle->ReleaseHandle();
}
