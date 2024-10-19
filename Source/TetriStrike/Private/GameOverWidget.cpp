// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Restart->OnClicked.AddDynamic(this, &UGameOverWidget::Restart);
	Button_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::Quit);
}

void UGameOverWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainLevel");
}

void UGameOverWidget::Quit()
{
	UWorld* CurrentWorld = GetWorld();

	UKismetSystemLibrary::QuitGame(CurrentWorld, CurrentWorld->GetFirstPlayerController(),
	                               EQuitPreference::Quit, false);
}
