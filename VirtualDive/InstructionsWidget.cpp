// Fill out your copyright notice in the Description page of Project Settings.


#include "InstructionsWidget.h"
UInstructionsWidget::UInstructionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
}
void UInstructionsWidget::NativeConstruct(){
    Super::NativeConstruct();
}

void UInstructionsWidget::OpenInstructionPanel(){
    UE_LOG(LogTemp, Warning, TEXT("PEYSAR"));
    if(AttachedUI()){
        IntroductionText->SetVisibility(ESlateVisibility::Visible);
        BackgroundImage->SetVisibility(ESlateVisibility::Visible);
        IntroductionText->SetText(FText::FromString(WriteInstructions()));
    }
}
void UInstructionsWidget::CloseInstructionPanel(){
    if(AttachedUI()){
        IntroductionText->SetVisibility(ESlateVisibility::Hidden);
        BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UInstructionsWidget::AttachedUI(){
    return (IntroductionText && BackgroundImage);
}
FString UInstructionsWidget::WriteInstructions(){
    FString text = "";
    text+="Hello! Welcome to the Virtual Dive Experience Simulator! \n";
    text+="Get up close to the animals to obtain details about them! \n";
    text+="Take a screenshot with 'M'\n";
    text+="Toggle the records journal with 'P' \n";
    text+="Toggle this instruction panel with 'L' \n";
    text+=" Have fun! \n";
    return text;
}

