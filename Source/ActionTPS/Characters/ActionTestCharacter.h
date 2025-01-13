// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionCharacterBase.h"
#include "ActionTestCharacter.generated.h"

UCLASS()
class ACTIONTPS_API AActionTestCharacter : public AActionCharacterBase
{
	GENERATED_BODY()

public:
	AActionTestCharacter();
	
	// Returns owning ActionAttributeSet
	virtual UActionAttributeSetBase* GetActionAttributeSet() const override;
	
protected:

	TObjectPtr<UActionAttributeSetBase> ActionAttributeSetBase;
};
