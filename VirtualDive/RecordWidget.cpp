// Fill out your copyright notice in the Description page of Project Settings.


#include "RecordWidget.h"

URecordWidget::URecordWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
}
void URecordWidget::NativeConstruct(){
    Super::NativeConstruct();
}

void URecordWidget::DisplayAnimal(FString Name, FString Tag,FString Description, FString ImageLink){
    AnimalTitle->SetText(FText::FromString(Name));
    AnimalDescription->SetText(FText::FromString(Description));
    AnimalImage->SetBrush(*UBook::SpeciesImageDictionary[Tag].Get());
}

void URecordWidget::ActivateLeftArrow(){
    if(AttachedUI()) LeftArrowImage->SetVisibility(ESlateVisibility::Visible);
}
void URecordWidget::ActivateRightArrow(){
    if(AttachedUI()) RightArrowImage->SetVisibility(ESlateVisibility::Visible);
}


void URecordWidget::OpenInfo(){
    if(AttachedUI()){
        AnimalDescription->SetVisibility(ESlateVisibility::Visible);
        AnimalDescription->SetText(FText::FromString("Find an animal to learn info about them here!"));
        AnimalImage->SetVisibility(ESlateVisibility::Hidden);
        BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
    }
}
void URecordWidget::OpenBook(){
    if(AttachedUI()){
        AnimalTitle->SetVisibility(ESlateVisibility::Visible);
        AnimalDescription->SetVisibility(ESlateVisibility::Visible);
        AnimalImage->SetVisibility(ESlateVisibility::Visible);
        BackgroundImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void URecordWidget::CloseBook(){
    if(AttachedUI()){
        AnimalTitle->SetVisibility(ESlateVisibility::Hidden);
        AnimalDescription->SetVisibility(ESlateVisibility::Hidden);
        AnimalImage->SetVisibility(ESlateVisibility::Hidden);
        BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
        LeftArrowImage->SetVisibility(ESlateVisibility::Hidden);
        RightArrowImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool URecordWidget::AttachedUI(){
    return (AnimalTitle && AnimalDescription && AnimalImage && BackgroundImage && LeftArrowImage && RightArrowImage);
}