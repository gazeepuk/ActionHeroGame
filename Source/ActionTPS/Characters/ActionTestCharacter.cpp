// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionTestCharacter.h"

#include "AbilitySystem/AttributeSet/ActionAttributeSetBase.h"

AActionTestCharacter::AActionTestCharacter()
{
	ActionAttributeSetBase = CreateDefaultSubobject<UActionAttributeSetBase>(TEXT("ActionAttributeSetBase"));
}

UActionAttributeSetBase* AActionTestCharacter::GetActionAttributeSet() const
{
	return ActionAttributeSetBase;
}
