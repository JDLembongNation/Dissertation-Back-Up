// Fill out your copyright notice in the Description page of Project Settings.
/*
	ATTACH THIS ACTOR COMPONENT TO THE MAIN CHARACTER IN THE SCENE.
*/

#include "Book.h"

// Sets default values for this component's properties
UBook::UBook()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UBook::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}
TMap<FString, UBook::Species> UBook::SpeciesDictionary; //Tag to Species Struct dictionary
TMap<FString, TSharedPtr<FSlateDynamicImageBrush>> UBook::SpeciesImageDictionary; //Tag to Brush Image dictionary



// Called every frame
void UBook::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBook::GetEntryFromTag(FString identifier, Species& specimen){ //identifier is the tag.
	specimen = SpeciesDictionary.FindOrAdd(identifier); //If doesnt work change key to int32. 
}





