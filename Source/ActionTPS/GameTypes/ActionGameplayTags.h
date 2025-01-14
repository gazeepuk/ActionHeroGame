#pragma once

#include "NativeGameplayTags.h"

namespace ActionGameplayTags
{
	/** Input Tags **/
	// Native
    ACTIONTPS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
    ACTIONTPS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	// ~Native

	// Ability Input
    ACTIONTPS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Ability_NormalAttack);
	// ~Ability Input
	
	/* Event Tags */
	ACTIONTPS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_NormalAttack_Magician);

	/** Ability Tags **/
	ACTIONTPS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_NormalAttack_Magician);

}