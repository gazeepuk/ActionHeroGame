// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AActionProjectileBase::AActionProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AActionProjectileBase::SetProjectileGameplayEffectSpec(const FGameplayEffectSpec& InGameplayEffectSpec)
{
	ProjectileGameplayEffectSpec = InGameplayEffectSpec;
}

void AActionProjectileBase::ServerLaunchProjectileForward_Implementation(float InLaunchSpeed)
{
	LaunchProjectileForward(InLaunchSpeed);
	ClientLaunchProjectileForward(InLaunchSpeed);
}

void AActionProjectileBase::ClientLaunchProjectileForward_Implementation(float InLaunchSpeed)
{
	LaunchProjectileForward(InLaunchSpeed);
}

void AActionProjectileBase::LaunchProjectileForward(float InLaunchSpeed)
{
	ProjectileMovementComponent->MaxSpeed = InLaunchSpeed;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * InLaunchSpeed;
}

void AActionProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
	
}

void AActionProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AActionProjectileBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


