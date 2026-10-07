// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <Physics/ABCollision.h>

// Sets default values
AABItemBox::AABItemBox()
{
	// 컴포넌트 생성.
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	// 컴포넌트 계층 설정.
	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	// 박스 컴포넌트에 콜리전 프로필 설정.
	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// 박스 컴포넌트의 BeginOverlap 델리게이트에 함수 등록.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);

	// 박스 충돌 영역 크기 조정.
	Trigger->SetBoxExtent(FVector(40.0f, 45.0f, 32.0f));

	// 박스 스태틱 메시 애셋 로드.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(
		TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1")
	);

	if (BoxMeshRef.Succeeded())
	{
		// 로드한 스태틱 메시 애셋 설정.
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	// 위치 조정 (에디터에서 조정 후 데이터 설정).
	Mesh->SetRelativeLocation(FVector(0.0f, -3.0f, -30.0f));

	// 콜리전 끄기.
	Mesh->SetCollisionProfileName(CPROFILE_NOCOLLISION);

	// 파티클 애셋 로드.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(
		TEXT("/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh")
	);

	if (EffectRef.Succeeded())
	{
		// 로드한 파티클 이펙트 애셋 설정.
		Effect->SetTemplate(EffectRef.Object);

		// 바로 재생하지 않도록 설정.
		Effect->bAutoActivate = false;
	}
}

void AABItemBox::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// 파티클 재생.
	Effect->Activate();

	// 메시 컴포넌트 끄기.
	Mesh->SetHiddenInGame(true);

	// 액터 콜리전 끄기.
	SetActorEnableCollision(false);

	// 파티클 재생 종료 시 호출되는 델리게이트에 함수 등록.
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	// 액터 제거.
	Destroy();
}
