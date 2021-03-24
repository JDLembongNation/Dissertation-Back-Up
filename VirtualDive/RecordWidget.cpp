// Fill out your copyright notice in the Description page of Project Settings.


#include "RecordWidget.h"

URecordWidget::URecordWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
}
void URecordWidget::NativeConstruct(){
    Super::NativeConstruct();
}

void URecordWidget::DisplayAnimal(FString Name, FString Description, FString ImageLink){
    AnimalTitle->SetText(FText::FromString(Name));
    AnimalDescription->SetText(FText::FromString(Description));
}

void URecordWidget::OpenBook(){
    if(AttachedUI()){
        AnimalTitle->SetVisibility(ESlateVisibility::Visible);
        AnimalDescription->SetVisibility(ESlateVisibility::Visible);
        AnimalImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void URecordWidget::CloseBook(){
    if(AttachedUI()){
        AnimalTitle->SetVisibility(ESlateVisibility::Hidden);
        AnimalDescription->SetVisibility(ESlateVisibility::Hidden);
        AnimalImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool URecordWidget::AttachedUI(){
    return (AnimalTitle && AnimalDescription && AnimalImage);
}