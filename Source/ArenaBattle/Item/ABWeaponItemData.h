// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()
	
public:
	// 제공할 무기에 대한 스켈레탈 메시 애샛 변수.
	UPROPERTY(EditAnywhere, Category = Weapon)
	//TObjectPtr<class USkeletalMesh> WeaponMesh;
	TSoftObjectPtr<class USkeletalMesh> WeaponMesh;
};
