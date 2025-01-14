// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "ActionProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ACTIONTPS_API AActionProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AActionProjectileBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetProjectileGameplayEffectSpec(const FGameplayEffectSpec& InGameplayEffectSpec);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Projectile")
	void ServerLaunchProjectileForward(float InLaunchSpeed);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Projectile")
	void ClientLaunchProjectileForward(float InLaunchSpeed);
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void LaunchProjectileForward(float InLaunchSpeed);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FGameplayEffectSpec ProjectileGameplayEffectSpec;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComponent;
};
