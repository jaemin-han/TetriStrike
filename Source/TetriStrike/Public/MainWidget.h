// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RadialSlider.h"
#include "Components/ScaleBox.h"
#include "Components/CanvasPanel.h"
#include "MainWidget.generated.h"

/**
 * 
 */
class UTP_WeaponComponent;
UCLASS()
class TETRISTRIKE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class URadialSlider* RadialSlider;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	UTP_WeaponComponent* WeaponComponent;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UpdateRadialSlider();
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
};
