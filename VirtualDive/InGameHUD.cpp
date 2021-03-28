// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

AInGameHUD::AInGameHUD(){

}

void AInGameHUD::BeginPlay(){
    Super::BeginPlay();
    if(RecordWidgetClass){
        RecordWidget = CreateWidget<URecordWidget>(GetWorld(), RecordWidgetClass);
        if(RecordWidget){
            RecordWidget->AddToViewport();
        }
    }
    if(AnimalNotificationWidgetClass){
        AnimalNotificationWidget = CreateWidget<UAnimalNotificationWidget>(GetWorld(), AnimalNotificationWidgetClass);
        if(AnimalNotificationWidget){
            AnimalNotificationWidget->AddToViewport();
        }
    }
}

void AInGameHUD::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
}

void AInGameHUD::DrawHUD(){
    Super::DrawHUD();
}
void AInGameHUD::DisplayAnimal(FString Name, FString Tag,FString Description, FString ImageLink){
    if(RecordWidget){
        RecordWidget->DisplayAnimal(Name, Tag,Description, ImageLink);
    }
}
void AInGameHUD::OpenBook(){
    if(RecordWidget){
        RecordWidget->OpenBook();
    }
} 
void AInGameHUD::OpenInfo(){
    if(RecordWidget){
        RecordWidget->OpenInfo();
    }
}
void AInGameHUD::CloseBook(){
    if(RecordWidget){
        RecordWidget->CloseBook();
    }
} 
void AInGameHUD::ActivateLeftArrow(){
    if(RecordWidget){
        RecordWidget->ActivateLeftArrow();
    }
}
void AInGameHUD::ActivateRightArrow(){
    if(RecordWidget){
        RecordWidget->ActivateRightArrow();
    }
}
void AInGameHUD::GenerateRemainingAnimals(){
    if(AnimalNotificationWidget){
        AnimalNotificationWidget->GenerateRemainingAnimals();
    }
}
void AInGameHUD::DisplayNotification(){
    if(AnimalNotificationWidget){
        AnimalNotificationWidget->DisplayNotification();
    }
}
void AInGameHUD::CloseNotification(){
    if(AnimalNotificationWidget){
        AnimalNotificationWidget->CloseNotification();
    }
}
void AInGameHUD::RemoveAnimalFromList(FString Name){
    if(AnimalNotificationWidget){
        AnimalNotificationWidget->RemoveAnimalFromList(Name);
    }
}