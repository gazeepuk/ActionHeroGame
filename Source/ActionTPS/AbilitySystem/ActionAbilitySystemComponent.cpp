// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ActionAbilitySystemComponent.h"

void UActionAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if(!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& ActivatableAbility : GetActivatableAbilities())
	{
		if(!ActivatableAbility.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			continue;
		}
		TryActivateAbility(ActivatableAbility.Handle);
	}
}

void UActionAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}