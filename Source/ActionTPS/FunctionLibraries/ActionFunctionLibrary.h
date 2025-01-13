// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActionFunctionLibrary.generated.h"

class UActionAbilitySystemComponent;
enum class EActionConfirmType : uint8;
/**
 * 
 */
UCLASS()
class ACTIONTPS_API UActionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UActionAbilitySystemComponent* NativeGetActionAbilitySystemComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "ActionTPS|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "ActionTPS|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag InGameplayTag);

	UFUNCTION(BlueprintCallable, Category = "ActionTPS|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveType(AActor* InActor, FGameplayTag InGameplayTag, EActionConfirmType& OutConfirmType);
};
