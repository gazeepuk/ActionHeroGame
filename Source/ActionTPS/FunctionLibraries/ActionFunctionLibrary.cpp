// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "GameTypes/ActionEnumTypes.h"

UActionAbilitySystemComponent* UActionFunctionLibrary::NativeGetActionAbilitySystemComponentFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UActionAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UActionFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionAbilitySystemComponentFromActor(InActor);
	if(!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UActionFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionAbilitySystemComponentFromActor(InActor);
	if(!ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->AddLooseGameplayTag(TagToRemove);
	}	
}

bool UActionFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag InGameplayTag)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionAbilitySystemComponentFromActor(InActor);
	return ASC->HasMatchingGameplayTag(InGameplayTag);	
}

void UActionFunctionLibrary::BP_DoesActorHaveType(AActor* InActor, FGameplayTag InGameplayTag,
	EActionConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, InGameplayTag) ? EActionConfirmType::Yes : EActionConfirmType::No;
}
