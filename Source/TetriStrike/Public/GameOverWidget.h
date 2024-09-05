// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class TETRISTRIKE_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Restart;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Quit;

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void Restart();

	UFUNCTION()
	void Quit();
};
