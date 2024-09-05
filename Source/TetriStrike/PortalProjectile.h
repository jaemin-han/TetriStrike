// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PortalProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class TETRISTRIKE_API APortalProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;
	
	APortalProjectile();
	
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
protected:
	virtual void BeginPlay() override;
	
public:
	void SetDamage(float DamageAmount);
	void CalculateVelocity();
	float VelocityMultiplier = 150.0f;
	float VelocityMinimum = 300.0f;
	float ImpulseMultiplier = 700.0f;
	int32 ReverseMaker = 1;
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal Creation")
	FVector PortalLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal Creation")
	FRotator PortalRotation;
};
