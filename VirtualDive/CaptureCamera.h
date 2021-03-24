// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	TSet<FString> SeenAnimals;
	void FindActors();
	bool IsCameraActive = false;
	UInputComponent* InputComponent = nullptr;
	void ToggleCamera();
	UFUNCTION(BlueprintCallable, Category="Capture")
	void CaptureShot();
	TArray<AActor*> FoundActors;
	float range = 1000.f;
	TArray<UBook::Species>Specimen;
};

