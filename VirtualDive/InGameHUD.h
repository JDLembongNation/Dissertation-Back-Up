// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "RecordWidget.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALDIVE_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	public: 
	AInGameHUD();

	virtual void DrawHUD() override; 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override; 
	UFUNCTION()
	void DisplayAnimal(FString Name, FString Description, FString ImageLink);
	UFUNCTION()
	void OpenBook();
	UFUNCTION()
	void CloseBook();
	UFUNCTION()
	void OpenInfo();
	UFUNCTION()
	void ActivateLeftArrow();
	UFUNCTION()
	void ActivateRightArrow();

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> RecordWidgetClass;
private: 
	URecordWidget* RecordWidget;

};
