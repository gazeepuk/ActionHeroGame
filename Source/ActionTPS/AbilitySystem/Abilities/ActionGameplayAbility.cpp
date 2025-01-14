// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ActionGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "Characters/ActionCharacterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

AActor* UActionGameplayAbility::SpawnActorByClass(TSubclassOf<AActor> InActorClass, const FTransform& InActorTransform, bool bSpawnOnlyOnServer)
{
	if(!GetWorld())
	{
		return nullptr;
	}

	const FGameplayAbilityActivationInfo AbilityActivationInfo = GetCurrentActivationInfo();

	// Returns nullptr if spawns on Client and bSpawnOnlyOnServer is true
	if(bSpawnOnlyOnServer && !HasAuthority(&AbilityActivationInfo))
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetActionCharacterFromAvatarActor();
	SpawnParameters.Owner = GetActionCharacterFromAvatarActor();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor(InActorClass, &InActorTransform, SpawnParameters);
	
	return SpawnedActor;
}

AActionProjectileBase* UActionGameplayAbility::SpawnProjectileByClass(TSubclassOf<AActionProjectileBase> InProjectileClass,
                                                                      const FTransform& InProjectileTransform, const float InMaxSpeed, const float InGravityScale, const FGameplayEffectSpec&
                                                                      InProjectileGameplayEffectSpec, bool bSpawnOnlyOnServer)
{
	if(!GetWorld())
	{
		return nullptr;
	}

	const FGameplayAbilityActivationInfo AbilityActivationInfo = GetCurrentActivationInfo();
	if(bSpawnOnlyOnServer && !HasAuthority(&AbilityActivationInfo))
	{
		return nullptr;
	}
	
AActionProjectileBase* ProjectileBase = GetWorld()->SpawnActorDeferred<AActionProjectileBase>(
		InProjectileClass,
		InProjectileTransform,
		GetActionCharacterFromAvatarActor(),
		GetActionCharacterFromAvatarActor(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	ProjectileBase->SetProjectileGameplayEffectSpec(InProjectileGameplayEffectSpec);

	ProjectileBase->ProjectileMovementComponent->ProjectileGravityScale = InGravityScale;
	
	ProjectileBase->FinishSpawning(InProjectileTransform);

	if(HasAuthority(&AbilityActivationInfo))
	{
		ProjectileBase->LaunchProjectileForward(InMaxSpeed);
		ProjectileBase->ClientLaunchProjectileForward(InMaxSpeed);
	}
	else
	{
		ProjectileBase->ServerLaunchProjectileForward(InMaxSpeed);
	}
	
	return ProjectileBase;
}

AActionCharacterBase* UActionGameplayAbility::GetActionCharacterFromAvatarActor() const
{
	return Cast<AActionCharacterBase>(GetAvatarActorFromActorInfo());
}

UActionAbilitySystemComponent* UActionGameplayAbility::GetActionAbilitySystemComponentFromAvatarActor() const
{
	return Cast<UActionAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
