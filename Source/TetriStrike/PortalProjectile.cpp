// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalProjectile.h"

#include "BlendSpaceAnalysis.h"
#include "TetriStrikeCharacter.h"
#include "TetriStrikeGameMode.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortalProjectile::APortalProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APortalProjectile::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 1.0f;
}

void APortalProjectile::BeginPlay()
{
	Super::BeginPlay();

	CalculateVelocity();
}

void APortalProjectile::CalculateVelocity()
{
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
}

void APortalProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	PortalLocation = Hit.Location;
	PortalRotation = OtherActor->GetActorRotation();
	ATetriStrikeGameMode* gm = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());
	if (gm->PortalLocation != PortalLocation)
	{
		gm->PortalLocation = PortalLocation;
		gm->PortalRotation = PortalRotation;
		gm->bTransformCheck = true;
	}

	if (OtherActor->ActorHasTag(FName("Floor")))
	{
		gm->PortalType = EPortalType::Floor;
	}
	else if (OtherActor->ActorHasTag(FName("Front")))
	{
		gm->PortalType = EPortalType::Front;
	}
	else if (OtherActor->ActorHasTag(FName("Right")))
	{
		gm->PortalType = EPortalType::Right;
	}
	else if (OtherActor->ActorHasTag(FName("Left")))
	{
		gm->PortalType = EPortalType::Left;
	}
	else if (OtherActor->ActorHasTag(FName("Back")))
	{
		gm->PortalType = EPortalType::Back;
	}
	else
	{
		gm->PortalType = EPortalType::Not_Valid;
	}
	// Trying to Make Portal Through weapon component
	// TArray<AActor*> FoundActors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
	// for(auto actor : FoundActors)
	// {
	// 	UTP_WeaponComponent* Gun = actor->FindComponentByClass<UTP_WeaponComponent>();
	// 	if(Gun && Gun->bIsPortalGun)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Works Till here"));
	// 		Gun->PortalLocation = PortalLocation;
	// 		Gun->PortalRotation = PortalRotation;
	// 	}
	// }
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		Destroy();
	}
}

void APortalProjectile::SetDamage(float DamageAmount)
{
	Damage = DamageAmount;
	CalculateVelocity();
}
