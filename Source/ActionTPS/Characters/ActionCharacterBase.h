// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "ActionCharacterBase.generated.h"

class UDataAsset_StartUpData;
class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;
class UActionAttributeSetBase;
class UAttributeSet;
class UActionAbilitySystemComponent;

UCLASS(Abstract)
class ACTIONTPS_API AActionCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacterBase();
	
	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	//~ Begin Getters for ActionAttributeSet
	virtual UActionAttributeSetBase* GetActionAttributeSet() const;

	UAttributeSet* GetAttributeSet() const;

	template<class T>
	T* GetAttributeSet() const
	{
		return Cast<T>(GetAttributeSet());
	}
	//~ End Getters for ActionAttributeSet
	
	//~ Begin APawn Interface.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

	UFUNCTION(BlueprintPure, Category = "Combat|Trace")
	FVector GetCameraViewTraceLocations(float InDistance);
	UFUNCTION(BlueprintCallable, Category = "Combat|Trace")
	void TraceSingleHitResultFromLocationToCameraView(FHitResult& OutHitResult, const FVector& InTraceStart, const bool bTraceComplex, const TArray<AActor*>& InIgnoredActors, float InDistance = 50000.f);
	UFUNCTION(BlueprintCallable, Category = "Combat|Trace")
	void TraceMultiHitResultsFromLocationToCameraView(TArray<FHitResult>& OutHitResult, const FVector& InTraceStart, const bool bTraceComplex, const TArray<AActor*>& InIgnoredActors, float InDistance = 50000.f);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Combat")
	FVector GetCombatSocketLocation();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	virtual void OnRep_PlayerState() override;
	
	//~ Components
	TObjectPtr<UActionAbilitySystemComponent> ActionAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> CameraBoom;
	

	//~ DataAssets
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta=(AllowPrivateAccess))
	TObjectPtr<UDataAsset_InputConfig> DataAsset_InputConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta=(AllowPrivateAccess))
	TSoftObjectPtr<UDataAsset_StartUpData> DataAsset_StartUpData;
	
	//~ Input

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
	void Input_Move(const FInputActionValue& InputActionValue);	
	void Input_Look(const FInputActionValue& InputActionValue);
};
