// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ActionAttributeSetBase.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class ACTIONTPS_API UActionAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	// Health
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHealth")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, CurrentHealth);
	UPROPERTY(ReplicatedUsing = "OnRep_MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, MaxHealth);
	
	// Ultimate
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentUltimate")
	FGameplayAttributeData CurrentUltimate;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, CurrentUltimate);
	UPROPERTY(ReplicatedUsing = "OnRep_MaxUltimate")
	FGameplayAttributeData MaxUltimate;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, MaxUltimate);

	// Movement Speed
	UPROPERTY(ReplicatedUsing = "OnRep_MaxWalkSpeed")
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, MaxWalkSpeed);

	// DamageMultiplier
	UPROPERTY(ReplicatedUsing = "OnRep_DamageMultiplier")
	FGameplayAttributeData DamageMultiplier;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, DamageMultiplier);

	// DamageReduction
	UPROPERTY(ReplicatedUsing = "OnRep_DamageReduction")
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, DamageReduction);

	// Incoming Damage
	UPROPERTY(ReplicatedUsing = "OnRep_IncomingDamage")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UActionAttributeSetBase, IncomingDamage);

private:
	virtual void HandleMaxWalkSpeedAttributeChanges(const FGameplayEffectModCallbackData& Data);

	// Attributes' RepNotifies
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_CurrentUltimate(const FGameplayAttributeData& OldCurrentUltimate);
	UFUNCTION()
	virtual void OnRep_MaxUltimate(const FGameplayAttributeData& OldMaxUltimate);

	UFUNCTION()
	virtual void OnRep_MaxWalkSpeed(const FGameplayAttributeData& OldMaxWalkSpeed);

	UFUNCTION()
	virtual void OnRep_DamageMultiplier(const FGameplayAttributeData& OldDamageMultiplier);

	UFUNCTION()
	virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction);

	UFUNCTION()
	virtual void OnRep_IncomingDamage(const FGameplayAttributeData& OldIncomingDamage);
};
