// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "TetriStrikeCharacter.h"
#include "TetriStrikeProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Projects.h"
#include "SNegativeActionButton.h"
#include "TetriStrikeReverseProjectile.h"
#include "PortalProjectile.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MainWidget.h"
#include "TetriStrikeGameMode.h"
#include "PortalSpawner.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<ATetriStrikeProjectile> ProjectileBPClass(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonProjectile"));
	ProjectileClass = ProjectileBPClass.Class;

	static ConstructorHelpers::FClassFinder<ATetriStrikeReverseProjectile> ReverseProjectileBPClass(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonProjectileReverse"));
	ReverseProjectileClass = ReverseProjectileBPClass.Class;
	//BulletDamage = 1.0f;
}

float UTP_WeaponComponent::BulletDamage = 1.0f;

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position

			//bug temp
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Try and play the sound if specified
			if (bIsPortalGun)
			{
				if (PortalGunFireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, PortalGunFireSound, Character->GetActorLocation());
				}
			}
			else
			{
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
				}
			}

			// Try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}

			//Check Portal Function and Spawn Portal Projectile
			if (bIsPortalGun)
			{
				APortalProjectile* PortalProjectile = World->SpawnActor<APortalProjectile>(
					PortalProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if (PortalProjectile)
				{
					PortalLocation = PortalProjectile->PortalLocation;
					PortalRotation = PortalProjectile->PortalRotation;
					PortalProjectile->SetDamage(BulletDamage);
					PortalProjectile->ProjectileMovement->bRotationFollowsVelocity = true;
					PortalProjectile->SetActorRotation(SpawnRotation);
				}
				bIncreaseStart = false;
				SpawnPortal();
				return;
			}

			// Spawn the projectile at the muzzle
			ATetriStrikeProjectile* Projectile = World->SpawnActor<ATetriStrikeProjectile>(
				ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			//World->SpawnActor<ATetriStrikeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Projectile)
			{
				Projectile->SetDamage(BulletDamage);
				Projectile->ProjectileMovement->bRotationFollowsVelocity = true;
				Projectile->SetActorRotation(SpawnRotation);
			}
			bIncreaseStart = false;
		}
	}
	BulletDamage = 1.0f;
}

void UTP_WeaponComponent::ReverseFire()
{
	if (Character == nullptr || Character->GetController() == nullptr || bIsPortalGun)
	{
		return;
	}


	// Try and fire a projectile
	if (ReverseProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position

			//bug temp
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			//TSubclassOf<ATetriStrikeProjectile> ReverseProjectileClass = ATetriStrikeReverseProjectile::StaticClass();
			ATetriStrikeProjectile* Projectile = World->SpawnActor<ATetriStrikeProjectile>(
				ReverseProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			//World->SpawnActor<ATetriStrikeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Projectile)
			{
				Projectile->SetDamage(UTP_WeaponComponent::BulletDamage);
				Projectile->ProjectileMovement->bRotationFollowsVelocity = true;
				Projectile->SetActorRotation(SpawnRotation);
			}
			bIncreaseStart = false;
		}
	}
	UTP_WeaponComponent::BulletDamage = 1.0f;

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}


bool UTP_WeaponComponent::AttachWeapon(ATetriStrikeCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			// Fire
			//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::OnFireTriggered);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Ongoing, this,
			                                   &UTP_WeaponComponent::OnFireOngoing);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::Fire);

			EnhancedInputComponent->BindAction(ReverseFireAction, ETriggerEvent::Ongoing, this,
			                                   &UTP_WeaponComponent::OnReverseFireOngoing);
			EnhancedInputComponent->BindAction(ReverseFireAction, ETriggerEvent::Completed, this,
			                                   &UTP_WeaponComponent::ReverseFire);

			EnhancedInputComponent->BindAction(ToggleGun, ETriggerEvent::Started, this,
			                                   &UTP_WeaponComponent::ToggleGunFunction);
			EnhancedInputComponent->BindAction(DestroyPortal, ETriggerEvent::Started, this,
			                                   &UTP_WeaponComponent::PortalSeekAndDestroy);
		}
	}

	return true;
}

void UTP_WeaponComponent::ToggleGunFunction()
{
	if (bIsPortalGun)bIsPortalGun = false;
	else bIsPortalGun = true;
}

void UTP_WeaponComponent::SpawnPortal()
{
	ATetriStrikeGameMode* gm = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());
	if (gm->bTransformCheck)
	{
		APortalSpawner* PortalSpawn = GetWorld()->SpawnActor<APortalSpawner>(
			PortalSpawnFactory, gm->PortalLocation, gm->PortalRotation);
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTP_WeaponComponent::MyTimerFunction, 0.5f, false);
	}
	gm->bTransformCheck = false;
}

void UTP_WeaponComponent::PortalSeekAndDestroy()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor->GetName().Contains("Portal"))
		{
			Actor->Destroy();
		}
	}
	ATetriStrikeGameMode* gm = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		gm->PortalCount = 0;
	}
}


void UTP_WeaponComponent::MyTimerFunction()
{
	SpawnPortal();
}

void UTP_WeaponComponent::OnFireOngoing()
{
	bIncreaseStart = true;
}

void UTP_WeaponComponent::OnReverseFireOngoing()
{
	bIncreaseStart = true;
}


float UTP_WeaponComponent::GetBulletDamage() const
{
	return UTP_WeaponComponent::BulletDamage;
}


void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIncreaseStart)
	{
		IncreasePower();
	}
}

void UTP_WeaponComponent::SetMainUI(UMainWidget* InMainUI)
{
	MainUI = InMainUI;
}

void UTP_WeaponComponent::IncreasePower()
{
	if (UTP_WeaponComponent::BulletDamage < 100)
	{
		UTP_WeaponComponent::BulletDamage += IncreasePowerMultiplier * GetWorld()->GetDeltaSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Bulletdamage---- updated: %f"), UTP_WeaponComponent::BulletDamage);
	}
	else
	{
		UTP_WeaponComponent::BulletDamage = 100.0f;
	}
}
