// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABFountain.generated.h"

// 전방 선언.
class UStaticMeshComponent;

UCLASS()
class ARENABATTLE_API AABFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 컴포넌트 구성.
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Water;
};
