// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "Components/TextBlock.h"
#include "TetriStrike/TetriStrikeCharacter.h"
#include "TetriStrike/TetriStrikeGameMode.h"
#include "TetriStrike/TP_WeaponComponent.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<ATetriStrikeGameMode>(GetWorld()->GetAuthGameMode());
	PlayTime = GameMode->PlayTime;
}

void UMainWidget::UpdateRadialSlider()
{
	// UE_LOG(LogTemp, Warning, TEXT("test: %f"), WeaponComponent->BulletDamage);
	if (!WeaponComponent)
	{
		return;
	}
	if (!RadialSlider)
	{
		return;
	}
	if (WeaponComponent && RadialSlider)
	{
		//ATetriStrikeCharacter* test;
		ATetriStrikeCharacter* Character = Cast<ATetriStrikeCharacter>(GetOwningPlayerPawn());

		//float DamagePercent = Character->TP_WeaponComponent->GetBulletDamage() / 100.0f;
		//UE_LOG(LogTemp, Warning, TEXT("test: %f"),DamagePercent );

		//		float DamagePercentage = BulletDamage / 100.0f;
		float DamagePercent = UTP_WeaponComponent::BulletDamage / 100.0f;
		//float DamagePercent = WeaponComponent->GetBulletDamage() /100.0f;
		RadialSlider->SetValue(DamagePercent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponComponent is null!"));
	}
}

float UMainWidget::GetPlayTime()
{
	float NewTime = PlayTime - static_cast<float>(GetWorld()->GetTimeSeconds());
	return FMath::Max(NewTime, 0.0f);
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateRadialSlider();

	ScoreData->SetText(FText::AsNumber(GameMode->GetScore()));
	TimeData->SetText(FText::AsNumber(GetPlayTime()));
}
