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
	ProjectileMovementComponent->InitialSpeed =	0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AActionProjectileBase::SetProjectileGameplayEffectSpec(const FGameplayEffectSpec& InGameplayEffectSpec)
{
	ProjectileGameplayEffectSpec = InGameplayEffectSpec;
}

void AActionProjectileBase::SetProjectileSpeed(const float InMaxSpeed, const float InGravityScale)
{
	ProjectileMovementComponent->InitialSpeed = InMaxSpeed;
	ProjectileMovementComponent->MaxSpeed = InMaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = InGravityScale;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * InMaxSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), ProjectileMovementComponent->Velocity.Size()));
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


