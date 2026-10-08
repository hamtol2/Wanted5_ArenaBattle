// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// UFUNCTION 키워드 등록 필수.
	// Dynamic 타입의 델리게이트에는 UFUNCTION 필요함.
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 파티클 재생 종료 시 발생하는 델리게이트에 등록할 함수.
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

protected:
	// 충돌을 위한 박스 컴포넌트 (트리거).
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	// 스태틱 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// 파티클 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UParticleSystemComponent> Effect;

	// 아이템 정보.
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item;
};
