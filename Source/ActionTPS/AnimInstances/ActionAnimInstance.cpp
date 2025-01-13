// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/ActionCharacterBase.h"
#include "FunctionLibraries/ActionFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UActionAnimInstance::NativeInitializeAnimation()
{
	OwningActionCharacter = Cast<AActionCharacterBase>(TryGetPawnOwner());
	if(OwningActionCharacter)
	{
		OwningMovementComponent = OwningActionCharacter->GetCharacterMovement();
	}
}

void UActionAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if(!OwningActionCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningActionCharacter->GetVelocity().Size2D();
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningActionCharacter->GetVelocity(), OwningActionCharacter->GetActorRotation());

	if(bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}

bool UActionAnimInstance::DoesOwnerHasTag(FGameplayTag TagToCheck) const
{
	return UActionFunctionLibrary::NativeDoesActorHaveTag(GetOwningActor(), TagToCheck);
}
