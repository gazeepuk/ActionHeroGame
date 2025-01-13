#include "ActionStructTypes.h"

#include "AbilitySystem/Abilities/ActionGameplayAbility.h"

bool FActionInputAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
