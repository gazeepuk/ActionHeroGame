#pragma once

#include "GameplayTagContainer.h"
#include "ActionStructTypes.generated.h"


class UActionGameplayAbility;

USTRUCT(BlueprintType)
struct FActionInputAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTags"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UActionGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};