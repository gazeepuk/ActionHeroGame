// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ActionGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "Characters/ActionCharacterBase.h"
#include "Items/Projectiles/ActionProjectileBase.h"

void UActionGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if (AbilityActivationPolicy == EActionAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UActionGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EActionAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

AActor* UActionGameplayAbility::SpawnActorByClass(TSubclassOf<AActor> InActorClass, const FTransform& ActorTransform,
	AActor* InOwner, APawn* InInstigator, bool bSpawnOnlyOnServer)
{

	const FGameplayAbilityActivationInfo AbilityActivationInfo = GetCurrentActivationInfo();

	if(bSpawnOnlyOnServer && !HasAuthority(&AbilityActivationInfo))
	{
		return nullptr;
	}

	if(!GetWorld())
	{
		return nullptr;
	}

	AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(InActorClass, ActorTransform, InOwner, InInstigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	SpawnedActor->FinishSpawning(ActorTransform);

	return SpawnedActor;
}

AActionCharacterBase* UActionGameplayAbility::GetActionCharacterFromAvatarActor() const
{
	return Cast<AActionCharacterBase>(GetAvatarActorFromActorInfo());
}

UActionAbilitySystemComponent* UActionGameplayAbility::GetActionAbilitySystemComponentFromAvatarActor() const
{
	return Cast<UActionAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
