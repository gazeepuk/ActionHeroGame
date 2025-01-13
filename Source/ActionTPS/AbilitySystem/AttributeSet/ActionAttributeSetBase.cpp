// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/ActionAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UActionAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CurrentUltimate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxUltimate, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxWalkSpeed, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, IncomingDamage, COND_None, REPNOTIFY_Always);
}

void UActionAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		//Clamp CurrentHealth from 0.f to MaxHealth
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		// TODO: Death
	}
	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		//Clamp minimal value of MaxHealth to 0.f
		const float NewMaxHealth = FMath::Max(GetMaxHealth(), 0.f);
		SetMaxHealth(NewMaxHealth);
		
		//Clamp Current Health after MaxHealth changing
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));

		// TODO: Death
	}

	if(Data.EvaluatedData.Attribute == GetCurrentUltimateAttribute())
	{
		//Clamp CurrentUltimate from 0.f to MaxUltimate
		const float NewCurrentUltimate = FMath::Clamp(GetCurrentUltimate(), 0.f, GetMaxUltimate());
		SetCurrentUltimate(NewCurrentUltimate);
	}
	if(Data.EvaluatedData.Attribute == GetMaxUltimateAttribute())
	{
		//Clamp minimal value of MaxUltimate to 0.f
		const float NewMaxUltimate = FMath::Max(GetMaxUltimate(), 0.f);
		SetMaxUltimate(NewMaxUltimate);
		
		//Clamp Current Ultimate after MaxUltimate changing
		SetCurrentUltimate(FMath::Clamp(GetCurrentUltimate(), 0.f, GetMaxUltimate()));

		//TODO: Notify when CurrentUltimate == MaxUltimate
	}

	// Handle changes in HandleMaxWalkSpeedAttributeChanges function
	if(Data.EvaluatedData.Attribute == GetMaxWalkSpeedAttribute())
	{
		HandleMaxWalkSpeedAttributeChanges(Data);
	}
	
	// Clamp Damage Multiplier 
	if(Data.EvaluatedData.Attribute == GetDamageMultiplierAttribute())
	{
		SetDamageMultiplier(FMath::Max(GetDamageMultiplier(), 0.f));
	}
	// Clamp Damage Reduction
	if(Data.EvaluatedData.Attribute == GetDamageReductionAttribute())
	{
		SetDamageReduction(FMath::Max(GetDamageReduction(), 0.f));
	}
}

void UActionAttributeSetBase::HandleMaxWalkSpeedAttributeChanges(const FGameplayEffectModCallbackData& Data)
{
	// Set MaxWalkSpeed to Character Movement Component
	if(const ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor()))
	{
		const float NewMaxWalkSpeed = FMath::Max(0.f, GetMaxWalkSpeed());
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
	}
}

#pragma region RepNotifies

void UActionAttributeSetBase::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CurrentHealth, OldCurrentHealth);
}

void UActionAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

void UActionAttributeSetBase::OnRep_CurrentUltimate(const FGameplayAttributeData& OldCurrentUltimate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CurrentUltimate, OldCurrentUltimate);
}

void UActionAttributeSetBase::OnRep_MaxUltimate(const FGameplayAttributeData& OldMaxUltimate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxUltimate, OldMaxUltimate);
}

void UActionAttributeSetBase::OnRep_MaxWalkSpeed(const FGameplayAttributeData& OldMaxWalkSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxWalkSpeed, OldMaxWalkSpeed);

	// Change MaxWalkSpeed on Client
	if(const ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor()))
	{
		const float NewMaxWalkSpeed = FMath::Max(0.f, GetMaxWalkSpeed());
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
	}
}

void UActionAttributeSetBase::OnRep_DamageMultiplier(const FGameplayAttributeData& OldDamageMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageMultiplier, OldDamageMultiplier);
}

void UActionAttributeSetBase::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageReduction, OldDamageReduction);
}

void UActionAttributeSetBase::OnRep_IncomingDamage(const FGameplayAttributeData& OldIncomingDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, IncomingDamage, OldIncomingDamage);
}

#pragma endregion
