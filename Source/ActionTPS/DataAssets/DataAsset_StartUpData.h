// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameTypes/ActionStructTypes.h"
#include "DataAsset_StartUpData.generated.h"

class UGameplayEffect;
class UActionGameplayAbility;
class UActionAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONTPS_API UDataAsset_StartUpData : public UDataAsset
{
	GENERATED_BODY()

	
public:
	virtual void GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UActionGameplayAbility>> ActivateOnGivenAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UActionGameplayAbility>> ReactiveAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<FActionInputAbilitySet> InputAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;
	
	void GrantAbilities(const TArray<TSubclassOf<UActionGameplayAbility>>& InAbilitiesToGive, UActionAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
