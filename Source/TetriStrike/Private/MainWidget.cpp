// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "TetriStrike/TetriStrikeCharacter.h"
#include "TetriStrike/TP_WeaponComponent.h"

void UMainWidget::UpdateRadialSlider()
{
	UE_LOG(LogTemp, Warning, TEXT("test: %f"), WeaponComponent->BulletDamage);

	if(WeaponComponent && RadialSlider)
	{
		//ATetriStrikeCharacter* test;
		ATetriStrikeCharacter* Character = Cast<ATetriStrikeCharacter>(GetOwningPlayerPawn());

		//float DamagePercent = Character->TP_WeaponComponent->GetBulletDamage() / 100.0f;
		//UE_LOG(LogTemp, Warning, TEXT("test: %f"),DamagePercent );

		float DamagePercent = UTP_WeaponComponent::BulletDamage/100.0f;
		//float DamagePercent = WeaponComponent->GetBulletDamage() /100.0f;
		RadialSlider->SetValue(DamagePercent);

		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponComponent is null!"));
	}
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateRadialSlider();

}
