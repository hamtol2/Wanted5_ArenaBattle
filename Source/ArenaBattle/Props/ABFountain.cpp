// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ABFountain.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 객체 생성.
	// 생성자에서 추가하면 CDO에 생성됨 -> 자동으로 월드에 등록.
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	// 루트 컴포넌트 지정.
	RootComponent = Body;

	// 컴포넌트 계층 설정.
	Water->SetupAttachment(Body);

	// Water 컴포넌트의 상대 위치 설정.
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	// 애셋 검색 및 설정.
	// /Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FountainBodyMesh(
		TEXT("/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01")
	);

	// 로드에 성공하면 애셋 지정.
	if (FountainBodyMesh.Succeeded())
	{
		Body->SetStaticMesh(FountainBodyMesh.Object);
	}

	// Water 메시 검색 후 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FountainWaterMesh(
		TEXT("/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02")
	);

	// 로드에 성공하면 애셋 지정.
	if (FountainWaterMesh.Succeeded())
	{
		Water->SetStaticMesh(FountainWaterMesh.Object);
	}
}

// Called when the game starts or when spawned
void AABFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

