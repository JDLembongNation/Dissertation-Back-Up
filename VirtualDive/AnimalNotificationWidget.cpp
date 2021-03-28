// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalNotificationWidget.h"
UAnimalNotificationWidget::UAnimalNotificationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
}
void UAnimalNotificationWidget::NativeConstruct(){
    Super::NativeConstruct();
}

void UAnimalNotificationWidget::GenerateRemainingAnimals(){
    if(AttachedUI()){
        AnimalList->SetVisibility(ESlateVisibility::Visible);
        BackgroundImage->SetVisibility(ESlateVisibility::Visible);
        AnimalList->SetText(FText::FromString(GenerateStringList()));
    }
}

void UAnimalNotificationWidget::InitializeList(){
    for(const TPair<FString, UBook::Species>&pair: UBook::SpeciesDictionary){
            CheckList.Add(pair.Value.SpeciesName);
    }
    finishedLoading = true;
}
void UAnimalNotificationWidget::RemoveAnimalFromList(FString AnimalName){
    if(CheckList.Contains(AnimalName)) CheckList.Remove(AnimalName);    
}

FString UAnimalNotificationWidget::GenerateStringList(){
    if(UBook::SpeciesDictionary.Num() < NumberOfExhibits) {
        return TEXT("");
    }else if(!finishedLoading){
        InitializeList();
                return TEXT("");
    }else{
    FString list = "";
    for(FString f : CheckList){
        list+=f;
        list+=("\n");
    }
    return list;
    }
}

void UAnimalNotificationWidget::DisplayNotification(){
    if(AttachedUI()){
        AnimalNotification->SetVisibility(ESlateVisibility::Visible);
        AnimalNotification->SetText(FText::FromString("Animal Data Captured! Press P to see Details!"));
    }
}

void UAnimalNotificationWidget::CloseNotification(){
    if(AttachedUI()){
        AnimalNotification->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UAnimalNotificationWidget::AttachedUI(){
    return (AnimalNotification && AnimalList && BackgroundImage);
}