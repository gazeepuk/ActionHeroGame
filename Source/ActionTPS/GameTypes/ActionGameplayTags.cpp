#include "GameTypes/ActionGameplayTags.h"

namespace ActionGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_NormalAttack, "InputTag.Ability.NormalAttack");

	/* Event Tags */
	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_NormalAttack_Magician, "Event.Ability.NormalAttack.Magician");

	/** Ability Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Ability_NormalAttack_Magician, "Ability.NormalAttack.Magician");
	
}
