// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Book.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "BerwickshireRequestHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALDIVE_API UBerwickshireRequestHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBerwickshireRequestHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	TMap<int32, FString> ReferenceMap; //Combines Ref number with Tag Name
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FHttpModule* Http;
	/* The actual HTTP call */
	UFUNCTION()
	void CallToDatabase();

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ProcessJSON(TSharedPtr<FJsonObject> JsonObject);
	void CreateMap();
		
};
