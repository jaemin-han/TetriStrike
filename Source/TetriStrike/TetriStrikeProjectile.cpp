// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriStrikeProjectile.h"

#include "BlendSpaceAnalysis.h"
#include "TetriStrikeCharacter.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ATetriStrikeProjectile::ATetriStrikeProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATetriStrikeProjectile::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ATetriStrikeProjectile::BeginPlay()
{
	Super::BeginPlay();

	CalculateVelocity();
}

void ATetriStrikeProjectile::CalculateVelocity()
{
	/*
	if(!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is null"));
		return;
	}
	*/

	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);
	//FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector ForwardVector = GetActorForwardVector();
	float CalculatedSpeed = Damage * 100 + 300;
	if (CalculatedSpeed < 0.0f)
	{
		CalculatedSpeed = 0.0f;
	}
	FVector Velocity = ForwardVector * CalculatedSpeed;
	//ProjectileMovement->SetVelocityInLocalSpace(Velocity);

	ProjectileMovement->Velocity = Velocity;
	ProjectileMovement->InitialSpeed = CalculatedSpeed;
	ProjectileMovement->MaxSpeed = CalculatedSpeed;

	UE_LOG(LogTemp, Warning, TEXT("CalculatedSpeed: %f, Velocity: %s"), CalculatedSpeed, *Velocity.ToString());
}

void ATetriStrikeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		FVector CurrentVelocity = GetVelocity();
		//ReverseMaker = 1;
		UE_LOG(LogTemp, Warning, TEXT("ReverseMaker: %d"), ReverseMaker);
		FVector Impulse = CurrentVelocity * ImpulseMultiplier * ReverseMaker;

		// OtherComp->AddImpulseAtLocation(Impulse, GetActorLocation());
		OtherComp->AddImpulseAtLocation(Impulse, OtherComp->GetCenterOfMass());
		Destroy();
	}
}

void ATetriStrikeProjectile::SetDamage(float DamageAmount)
{
	Damage = DamageAmount;
	CalculateVelocity();
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), Damage);
}
