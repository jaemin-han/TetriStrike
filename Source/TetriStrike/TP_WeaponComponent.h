// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetriStrike.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class ATetriStrikeCharacter;
class UMainWidget;
class ATetriStrikeProjectile;
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TETRISTRIKE_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	
	// Select Projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class ATetriStrikeProjectile> ReverseProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class ATetriStrikeProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class APortalProjectile> PortalProjectileClass;

	/** Projectile class to spawn */
	//UPROPERTY(EditDefaultsOnly, Category=Projectile)
	//TSubclassOf<class ATetriStrikeProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* PortalGunFireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ReverseFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleGun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DestroyPortal;
	
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(ATetriStrikeCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReverseFire();

	//Change Gun's Function between portal and projectile
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ToggleGunFunction();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SpawnPortal();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PortalSeekAndDestroy();
	
	//void OnFireTriggered();
	void OnFireOngoing();
	void OnReverseFireOngoing();
	
	//void OnFireCompleted();
//
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetBulletDamage() const;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
//	float BulletDamage;
	static float BulletDamage;
	bool bIncreaseStart;

	void SetMainUI(UMainWidget* MainUI);

	void IncreasePower();

	//check gun projectile
	bool bIsPortalGun = false;

	//check portal existance
	bool bIsPortalCreated = false;

	//Portal transform
	UPROPERTY()
	FVector PortalLocation;

	UPROPERTY()
	FRotator PortalRotation;

	void MyTimerFunction();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APortalSpawner> PortalSpawnFactory;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//virtual void BeginPlay() override;
	
private:
	/** The Character holding this weapon*/
	ATetriStrikeCharacter* Character;

private:
//	float BulletDamage = 1.0f;
//	bool bIncreaseStart = false;

private:
	UMainWidget* MainUI;
	
};
