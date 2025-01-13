// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_StartUpData.h"

#include "GameplayEffect.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ActionGameplayAbility.h"

void UDataAsset_StartUpData::GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	// Apply StartUp GameplayEffects
	if(!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : StartUpGameplayEffects)
		{
			if(!GameplayEffect)
			{
				continue;
			}

			UGameplayEffect* EffectDefaultObject = GameplayEffect.GetDefaultObject();
			InASCToGive->ApplyGameplayEffectToSelf(EffectDefaultObject, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}

	// Grant Input Abilities
	for (const FActionInputAbilitySet& InputAbility : InputAbilities)
	{
		if(!InputAbility.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(InputAbility.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(InputAbility.InputTag);

		InASCToGive->GiveAbility(AbilitySpec);
	}
}

void UDataAsset_StartUpData::GrantAbilities(const TArray<TSubclassOf<UActionGameplayAbility>>& InAbilitiesToGive,
                                            UActionAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if(InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UActionGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
