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

	// 게이트(문) 컴포넌트/애셋 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(
		TEXT("/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE")
	);

	// 게이트 이름 및 위치 배치에 사용할 이름 값.
	static FName GateSockets[]
		= { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };

	// 루프로 컴포넌트 생성.
	for (FName GateSocket : GateSockets)
	{
		// 스태틱 메시 컴포넌트 생성.
		UStaticMeshComponent* Gate
			= CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);

		// 스태틱 메시 애셋 설정.
		if (GateMeshRef.Succeeded())
		{
			Gate->SetStaticMesh(GateMeshRef.Object);
		}

		// 속성 조정.
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocationAndRotation(
			FVector(0.0f, -80.0f, 0.0f),
			FRotator(0.0f, -90.0f, 0.0f)
		);

		// 맵(TMap)에 추가.
		Gates.Add(GateSocket, Gate);

		// 게이트 트리거 생성 및 설정.
		// 예: +XGateTrigger
		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger 
			= CreateDefaultSubobject<UBoxComponent>(TriggerName);

		// 속성 설정.
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

		// 충돌했을 때 발행되는 델리게이트에 함수 등록.
		GateTrigger->OnComponentBeginOverlap.AddDynamic(
			this, &AABStageGimmick::OnGateTriggerBeginOverlap
		);

		// 게이트 구분을 위해 태그 설정.
		GateTrigger->ComponentTags.Add(GateSocket);

		// 배열에 추가.
		GateTriggers.Add(GateTrigger);
	}

	// 시작 상태 설정.
	CurrentState = EStageState::Ready;

	// 상태에 따른 로직 분기를 위한 델리게이트 맵 구성.
	StateChangeActions.Add(
		EStageState::Ready,
		FOnStageChangedDelegate::CreateUObject(
			this, &AABStageGimmick::SetReady
		)
	);

	StateChangeActions.Add(
		EStageState::Fight,
		FOnStageChangedDelegate::CreateUObject(
			this, &AABStageGimmick::SetFight
		)
	);

	StateChangeActions.Add(
		EStageState::Reward,
		FOnStageChangedDelegate::CreateUObject(
			this, &AABStageGimmick::SetChooseReward
		)
	);

	StateChangeActions.Add(
		EStageState::Next,
		FOnStageChangedDelegate::CreateUObject(
			this, &AABStageGimmick::SetChooseNext
		)
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

void AABStageGimmick::SetState(EStageState InNewState)
{
	// 현재 상태 업데이트.
	CurrentState = InNewState;

	// 관련 델리게이트 호출.
	// 맵에 포함되어 있는지 확인.
	if (StateChangeActions.Contains(InNewState))
	{
		StateChangeActions[InNewState].ExecuteIfBound();
	}
}

void AABStageGimmick::SetReady()
{}

void AABStageGimmick::SetFight()
{}

void AABStageGimmick::SetChooseReward()
{}

void AABStageGimmick::SetChooseNext()
{}
