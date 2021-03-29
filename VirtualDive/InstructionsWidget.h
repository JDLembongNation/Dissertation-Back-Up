// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "InstructionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALDIVE_API UInstructionsWidget : public UUserWidget
{
	GENERATED_BODY()
	public: 
	UInstructionsWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* IntroductionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* BackgroundImage;
	void OpenInstructionPanel();
	void CloseInstructionPanel();
	bool AttachedUI();
	FString WriteInstructions();

};
