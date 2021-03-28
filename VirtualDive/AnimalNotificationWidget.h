// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BerwickshireRequestHandler.h"
#include "Runtime/UMG/Public/UMG.h"
#include "AnimalNotificationWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALDIVE_API UAnimalNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
		public: 
	UAnimalNotificationWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override; 
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AnimalNotification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AnimalList;
	
	void InitializeList();
	void GenerateRemainingAnimals(); //Will change the text in the AnimalList Box. Call only once speciesImageIDcitonary = Referencemap length
	void DisplayNotification(); 
	void RemoveAnimalFromList(FString AnimalName);
		void CloseNotification(); 
	TArray<FString> CheckList;
	bool AttachedUI();
	FString GenerateStringList();
	int32 NumberOfExhibits = 9;
	bool finishedLoading = false;
	
};
