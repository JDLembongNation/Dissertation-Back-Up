// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "RecordWidget.h"
#include "InstructionsWidget.h"
#include "AnimalNotificationWidget.h"
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
	void DisplayAnimal(FString Name, FString Tag,FString Description, FString ImageLink);
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
	UFUNCTION()
	void DisableLeftArrow();
	UFUNCTION()
	void DisableRightArrow();
	UFUNCTION()
	void GenerateRemainingAnimals(); //Will change the text in the AnimalList Box. Call only once speciesImageIDcitonary = Referencemap length
	UFUNCTION()
	void DisplayNotification(); 
	UFUNCTION()
	void CloseNotification();
	UFUNCTION()
	void RemoveAnimalFromList(FString Name); 
	UFUNCTION()
	void OpenInstructionPanel();
	UFUNCTION()
	void CloseInstructionPanel();

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> RecordWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> AnimalNotificationWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UUserWidget> InstructionsWidgetClass;
private: 
	URecordWidget* RecordWidget;
	UAnimalNotificationWidget* AnimalNotificationWidget;
	UInstructionsWidget* InstructionsWidget;

};
