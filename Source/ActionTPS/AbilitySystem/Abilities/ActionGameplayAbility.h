// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActionGameplayAbility.generated.h"

UENUM()
enum class EActionAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

class UActionAbilitySystemComponent;
class AActionCharacterBase;
/**
 * 
 */
UCLASS()
class ACTIONTPS_API UActionGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UFUNCTION(BlueprintCallable)
	AActor* SpawnActorByClass(TSubclassOf<AActor> InActorClass, const FTransform& ActorTransform, AActor* InOwner, APawn* InInstigator, bool bSpawnOnlyOnServer);
	
	UFUNCTION(BlueprintPure, Category = "ActionTPS|Abilitiy")
	AActionCharacterBase* GetActionCharacterFromAvatarActor() const;

	UFUNCTION(BlueprintPure, Category = "ActionTPS|Abilitiy")
	UActionAbilitySystemComponent* GetActionAbilitySystemComponentFromAvatarActor() const;

	UPROPERTY(EditDefaultsOnly, Category = "ActionTPS|Abilitiy")
	EActionAbilityActivationPolicy AbilityActivationPolicy{EActionAbilityActivationPolicy::OnTriggered};
};
