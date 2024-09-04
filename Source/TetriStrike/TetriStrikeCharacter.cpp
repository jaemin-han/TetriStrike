// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriStrikeCharacter.h"
#include "TetriStrikeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MainWidget.h"
#include "TetriStrikePlayerController.h"
#include "TP_WeaponComponent.h"
#include "MainWidget.h"
#include "NetworkMessage.h"
#include "TetriStrikePlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATetriStrikeCharacter

ATetriStrikeCharacter::ATetriStrikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	TP_WeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("TP_WeaponComponent"));
	TP_WeaponComponent->SetupAttachment(Mesh1P);
	
}

void ATetriStrikeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	SetFlyingMode();
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->MaxFlySpeed = 1200.0f;
		MovementComponent->BrakingDecelerationFlying = 7000.0f;
		//MovementComponent->SetMovementMode(MOVE_Flying);

		
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(MainUIFactory && PlayerController)
	{
		UMainWidget* MainUI = CreateWidget<UMainWidget>(PlayerController,MainUIFactory);
		if(MainUI)
		{
			MainUI->AddToViewport();
			MainUI->WeaponComponent = TP_WeaponComponent;

			if(TP_WeaponComponent)
			{
				TP_WeaponComponent->SetMainUI(MainUI);
			}
			//MainUI->UpdateRadialSlider();
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ATetriStrikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATetriStrikeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATetriStrikeCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATetriStrikeCharacter::SetFlyingMode()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}


void ATetriStrikeCharacter::Move(const FInputActionValue& Value)
{
	FVector MovementVector = Value.Get<FVector>();

	// input is a Vector2D
	//FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		AddMovementInput(GetActorUpVector(), MovementVector.Z);
	}
}

void ATetriStrikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

