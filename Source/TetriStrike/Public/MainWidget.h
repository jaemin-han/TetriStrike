// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RadialSlider.h"
#include "MainWidget.generated.h"

/**
 * 
 */
class UTP_WeaponComponent;

UCLASS()
class TETRISTRIKE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class URadialSlider* RadialSlider;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	UTP_WeaponComponent* WeaponComponent;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UpdateRadialSlider();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreData;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeData;

	// PlayTime
	UPROPERTY(EditAnywhere)
	float PlayTime;

	UFUNCTION()
	float GetPlayTime();

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	class ATetriStrikeGameMode* GameMode;
};
