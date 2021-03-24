// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Book.h"
#include "CaptureCamera.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALDIVE_API UCaptureCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCaptureCamera();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TSet<FString> SeenAnimals; //Record Animals that are seen here. 
	bool IsRecordViewable = false;
	FHitResult AnimalActor;
	bool IsCameraActive = false;
	UInputComponent* InputComponent = nullptr;
	TArray<UBook::Species> SpeciesList;
	TArray<AActor*> FoundActors;
	float Range = 1500.f; //Change if necessary. 
	FVector LineTraceEnd;
	UFUNCTION(BlueprintCallable, Category="Capture")
	void CaptureShot();

	FVector GetPlayerLocation(); 
	FRotator GetPlayerRotation();
	void ProcessSighting();
	void DetectAnimal(); 
	void ToggleRecords();
	void UpdateDetailsNext();
	void UpdateDetailsPrevious();
	int32 Reference = -1; //The book position in the speciesList.

};

