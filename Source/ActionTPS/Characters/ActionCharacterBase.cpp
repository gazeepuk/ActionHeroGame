#include "ActionCharacterBase.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/ActionAttributeSetBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "Components/Input/ActionEnhancedInputComponent.h"
#include "DataAssets/DataAsset_StartUpData.h"
#include "GameTypes/ActionGameplayTags.h"

#define ECC_Traceable ECC_GameTraceChannel1

// Sets default values
AActionCharacterBase::AActionCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->SetReceivesDecals(false);

	ActionAbilitySystemComponent = CreateDefaultSubobject<UActionAbilitySystemComponent>(TEXT("ActionAbilitySystemComponent"));
	ActionAbilitySystemComponent->SetIsReplicated(true);
	ActionAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

UAbilitySystemComponent* AActionCharacterBase::GetAbilitySystemComponent() const
{
	return ActionAbilitySystemComponent;
}

UActionAttributeSetBase* AActionCharacterBase::GetActionAttributeSet() const
{
	return nullptr;
}

UAttributeSet* AActionCharacterBase::GetAttributeSet() const
{
	return GetActionAttributeSet();
}

// Called when the game starts or when spawned
void AActionCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetAttributeSet(), TEXT("Attribute Set is NULL. Create one in the class contructor"));
}

//Init ASC on Client
void AActionCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if(ActionAbilitySystemComponent)
	{
		ActionAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

//Init ASC on Server
void AActionCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(ActionAbilitySystemComponent)
	{
		ActionAbilitySystemComponent->InitAbilityActorInfo(this, this);

		SetOwner(NewController);
		
		if(!HasAuthority())
		{
			return;
		}

		// Give GameplayAbilities and apply GameplayEffects on Server
		if(DataAsset_StartUpData.IsValid())
		{
			DataAsset_StartUpData->GiveToAbilitySystemComponent(ActionAbilitySystemComponent);	
		}
		else if(!DataAsset_StartUpData.IsNull())
		{
			if(UDataAsset_StartUpData* LoadedStartUpData = DataAsset_StartUpData.LoadSynchronous())
			{
				LoadedStartUpData->GiveToAbilitySystemComponent(ActionAbilitySystemComponent);
			}
		}
	}
}


FVector AActionCharacterBase::GetCameraViewTraceLocations(const float InDistance)
{
	if(!GetWorld())
	{
		return FVector::ZeroVector;
	}

	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FVector CameraForwardVector = CameraComponent->GetForwardVector();
	const FVector EndTraceLocation = CameraLocation + CameraForwardVector * InDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	FHitResult CameraHitResult;
	GetWorld()->LineTraceSingleByChannel(CameraHitResult, CameraLocation, EndTraceLocation, ECC_Traceable, CollisionQueryParams);

	return CameraHitResult.bBlockingHit ? CameraHitResult.Location : CameraHitResult.TraceEnd;
}

void AActionCharacterBase::TraceSingleHitResultFromLocationToCameraView(FHitResult& OutHitResult, const float InDistance, const FVector& InTraceStart, const bool bTraceComplex, const TArray<AActor*>& InIgnoredActors)
{
	if(!GetWorld())
	{
		return;
	}

	const FVector TraceEnd = GetCameraViewTraceLocations(InDistance);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(InIgnoredActors);

	GetWorld()->LineTraceSingleByChannel(OutHitResult, InTraceStart, TraceEnd, ECC_Traceable, CollisionQueryParams);	
}

void AActionCharacterBase::TraceMultiHitResultsFromLocationToCameraView(TArray<FHitResult>& OutHitResult, float InDistance,
	const FVector& InTraceStart, const bool bTraceComplex, const TArray<AActor*>& InIgnoredActors)
{
	if(!GetWorld())
	{
		return;
	}

	const FVector TraceEnd = GetCameraViewTraceLocations(InDistance);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(InIgnoredActors);

	GetWorld()->LineTraceMultiByChannel(OutHitResult, InTraceStart, TraceEnd, ECC_WorldDynamic, CollisionQueryParams);
}

FVector AActionCharacterBase::GetCombatSocketLocation_Implementation()
{
	return FVector::ZeroVector;
}

void AActionCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(DataAsset_InputConfig, TEXT("Input config DataAsset is nullptr"));

	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->AddMappingContext(DataAsset_InputConfig->DefaultMappingContext, 0);

	UActionEnhancedInputComponent* ActionEnhancedInputComponent = Cast<UActionEnhancedInputComponent>(PlayerInputComponent);
	checkf(ActionEnhancedInputComponent, TEXT("ActionEnhancedInputComponent is nullptr"));

	ActionEnhancedInputComponent->BindNativeInputAction(DataAsset_InputConfig, ActionGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ActionEnhancedInputComponent->BindNativeInputAction(DataAsset_InputConfig, ActionGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	ActionEnhancedInputComponent->BindAbilityInputAction(DataAsset_InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AActionCharacterBase::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	if(ActionAbilitySystemComponent)
	{
		ActionAbilitySystemComponent->OnAbilityInputPressed(InInputTag);	
	}
}

void AActionCharacterBase::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	if(ActionAbilitySystemComponent)
	{
		ActionAbilitySystemComponent->OnAbilityInputReleased(InInputTag);	
	}
}

void AActionCharacterBase::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AActionCharacterBase::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

