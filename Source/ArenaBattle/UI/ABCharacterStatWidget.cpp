// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include <Components/TextBlock.h>

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// FABCharacterStat이 가지는 속성을 리플렉션을 통해서 가져와 활용.
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());
		nullptr != PropIt;
		++PropIt
		)
	{
		// 속성의 이름 값 (키).
		const FName PropName(PropIt->GetName());

		// Base 스탯에 대한 텍스트 블록 이름.
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"),
			*PropIt->GetName()
		);

		// Modifier 스탯에 대한 텍스트 블록 이름.
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"),
			*PropIt->GetName()
		);
	}
}

void UABCharacterStatWidget::UpdateStat(
	const FABCharacterStat& InBaseStat, 
	const FABCharacterStat& InModifierStat)
{

}
