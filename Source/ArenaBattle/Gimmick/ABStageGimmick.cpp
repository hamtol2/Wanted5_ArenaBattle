// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include <Physics/ABCollision.h>

#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	// 스테이지 관련 설정.
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	// 스테이지 메시 애셋 로드 후 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(
		TEXT("/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE")
	);

	if (StageMeshRef.Succeeded())
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	// 스테이지 트리거 생성 및 설정.
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// 충돌 시 발생하는 델리게이트에 함수 등록.
	StageTrigger->OnComponentBeginOverlap.AddDynamic(
		this, &AABStageGimmick::OnStageTriggerBeginOverlap
	);
}

void AABStageGimmick::OnStageTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{

}

void AABStageGimmick::OnGateTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{

}
