// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include <Physics/ABCollision.h>
#include <Character/ABCharacterNonPlayer.h>
#include <Item/ABItemBox.h>

#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Engine/OverlapResult.h>

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

	// NPC 생성에 대기할 시간 값 (2초).
	OpponentSpawnTime = 2.0f;

	// NPC 생성에 사용할 타입(클래스) 설정.
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	// 보상 관련 변수 설정.
	RewardBoxClass = AABItemBox::StaticClass();

	// 생성할 위치 설정.
	for (FName GateSocket : GateSockets)
	{
		// 중심 위치와 문과의 거리의 1/2 위치.
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2.0f;

		// 생성 위치 배열에 추가.
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 상태 변경 테스트 가능하도록 함수 호출.
	SetState(CurrentState);
}

void AABStageGimmick::OnStageTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 스테이지에 진입을 하면 대전(Fight) 상태로 전환.
	SetState(EStageState::Fight);
}

void AABStageGimmick::OnGateTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 오버랩된 게이트에서 태그 확인.
	FName ComponentTag = OverlappedComponent->ComponentTags[0];

	// 컴포넌트 태그 값에서 앞에 두 글자만 자르기.
	// +XGate -> +X
	FName SocketName = *ComponentTag.ToString().Left(2);

	// 값 확인(스테이지 메시에 소켓이 있는지 확인).
	ensureAlways(Stage->DoesSocketExist(SocketName));

	// 생성할 위치(소켓 이름을 기준으로 생성 위치 가져오기).
	FVector NewLocation = Stage->GetSocketLocation(SocketName);

	// 충돌 결과를 전달 받을 변수.
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(GateTrigger), false, this);

	// 지나온 위치는 다시 못 가도록 확인.
	// 이미 스테이지 액터가 생성되어 있는데 재차 생성하지 않도록 방지.
	bool Result = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		FCollisionShape::MakeSphere(775.0f),
		Params
	);

	if (!Result)
	{
		// 스테이지 액터 생성.
		AABStageGimmick* NewGimmick
			= GetWorld()->SpawnActor<AABStageGimmick>(
				NewLocation, FRotator::ZeroRotator
			);

		// 스테이지 레벨 설정.
		NewGimmick->SetStageNum(CurrentStageNum + 1);
	}
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
{
	// 가운데 트리거(스테이지 트리거) 활성화.
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// 게이트와는 상호작용하지 않도록 콜리전 끄기.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_NOCOLLISION);
	}

	// 들어올 수 있게 문 열기.
	OpenAllGates();
}

void AABStageGimmick::SetFight()
{
	// 가운데 트리거(스테이지 트리거) 활성화.
	StageTrigger->SetCollisionProfileName(CPROFILE_NOCOLLISION);

	// 게이트와는 상호작용하지 않도록 콜리전 끄기.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_NOCOLLISION);
	}

	// 문 닫기.
	CloseAllGates();

	// NPC 생성.
	GetWorld()->GetTimerManager().SetTimer(
		OpponentTimerHandle,
		FTimerDelegate::CreateUObject(
			this, &AABStageGimmick::OnOpponentSpawn
		),
		OpponentSpawnTime,
		false
	);
}

void AABStageGimmick::SetChooseReward()
{
	// 가운데 트리거(스테이지 트리거) 활성화.
	StageTrigger->SetCollisionProfileName(CPROFILE_NOCOLLISION);

	// 게이트와는 상호작용하지 않도록 콜리전 끄기.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_NOCOLLISION);
	}

	// 문 닫기.
	CloseAllGates();

	// 보상 아이템 상자 생성.
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	// 가운데 트리거(스테이지 트리거) 활성화.
	StageTrigger->SetCollisionProfileName(CPROFILE_NOCOLLISION);

	// 게이트와는 상호작용하지 않도록 콜리전 끄기.
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	// 문 열기 -> 다른 스테이지로 이동할 수 있도록.
	OpenAllGates();
}

void AABStageGimmick::OpenAllGates()
{
	// 게이트 컴포넌트 맵을 순회하면서 회전 설정.
	for (auto Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	// 게이트 컴포넌트 맵을 순회하면서 회전 설정.
	for (auto Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	// NPC가 죽으면 보상 단계로 전환.
	SetState(EStageState::Reward);
}

void AABStageGimmick::OnOpponentSpawn()
{
	// NPC 생성 위치.
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;

	// NPC 액터 생성.
	AActor* OpponentActor = GetWorld()->SpawnActor(
		OpponentClass,
		&SpawnLocation,
		&FRotator::ZeroRotator
	);

	// 타입 확인.
	AABCharacterNonPlayer* ABOpponentCharacter
		= Cast<AABCharacterNonPlayer>(OpponentActor);
	if (ABOpponentCharacter)
	{
		// NPC가 죽었을 때 실행될 델리게이트에 함수 등록.
		OpponentActor->OnDestroyed.AddDynamic(
			this, &AABStageGimmick::OnOpponentDestroyed
		);

		// 현재 스테이지 레벨을 캐릭터 NPC의 레벨로 설정.
		ABOpponentCharacter->SetLevel(CurrentStageNum);
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 캐릭터가 아이템 상자를 습득하면 이 함수가 호출됨.
	// 아이템 상자 배열을 순회하면서 처리.
	for (const auto& RewardBox : RewardBoxes)
	{
		// 유효성 확인.
		if (RewardBox.IsValid())
		{
			// 저장해둔 값이 약참조 값이기 때문에 포인터 가져오기.
			AABItemBox* ValidItemBox = RewardBox.Get();

			// 캐릭터가 수집하지 않은 아이템 상자를 구분.
			AActor* OverlappedActor = OverlappedComponent->GetOwner();
			if (OverlappedActor != ValidItemBox)
			{
				// 삭제 처리. 캐릭터와 상호작용한 아이템 상자는 알아서 제거됨.
				ValidItemBox->Destroy();
			}
		}
	}

	// 다음 단계로 전환.
	SetState(EStageState::Next);
}

void AABStageGimmick::SpawnRewardBoxes()
{
	// 아이템 상자 생성 위치를 순회하면서 아이템 상자 생성.
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		// 생성 위치.
		FVector WorldSpawnLoation
			= GetActorLocation()
			+ RewardBoxLocation.Value
			+ FVector(0.0f, 0.0f, 30.0f);

		// 아이템 상자 액터 생성.
		AActor* ItemActor = GetWorld()->SpawnActor(
			RewardBoxClass,
			&WorldSpawnLoation,
			&FRotator::ZeroRotator
		);

		// 나중에 어떤 상자와 부딪혔는지 파악을 위해 태그 값 추가.
		ItemActor->Tags.Add(RewardBoxLocation.Key);

		// 아이템 상자 타입으로 형변환.
		AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			// 아이템 상자와 부딪혔을 때를 파악하기 위해 이벤트 연결.
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(
				this, &AABStageGimmick::OnRewardTriggerBeginOverlap
			);

			// 생성된 액터를 배열에 추가.
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}
