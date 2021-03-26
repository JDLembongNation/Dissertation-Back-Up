// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "RecordWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALDIVE_API URecordWidget : public UUserWidget
{
	GENERATED_BODY()
	public: 
	URecordWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override; 
	void DisplayAnimal(FString Name, FString Description, FString ImageLink);
	void OpenBook();
	void CloseBook();
	bool AttachedUI();
	void OpenInfo();
	void ActivateLeftArrow(); //If the player can go left, the left key will emerge.
	void ActivateRightArrow(); //If the player can go right, the right key will be shown.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AnimalTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AnimalDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* AnimalImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* BackgroundImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* LeftArrowImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* RightArrowImage;


};
