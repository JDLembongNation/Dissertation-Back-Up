// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "Styling/SlateBrush.h"
#include "Book.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALDIVE_API UBook : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBook();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	struct Species{
		FString SpeciesName;
		FString SpeciesTag;
		FString SpeciesDescription;
	};
	static TMap<FString, TSharedPtr<FSlateDynamicImageBrush>> SpeciesImageDictionary;
	static TMap<FString, Species> SpeciesDictionary;
	static void GetEntryFromTag(FString identifier, Species& specimen); //Tag is not the same as epciesName. need separate Map to convert the two.

};
