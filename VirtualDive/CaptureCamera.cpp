// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureCamera.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Book.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#define OUT //Readability --> Out Parameters. 

// Sets default values for this component's properties
UCaptureCamera::UCaptureCamera()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCaptureCamera::BeginPlay()
{
	Super::BeginPlay();

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		InputComponent->BindAction("CaptureScreenshot",IE_Pressed, this, &UCaptureCamera::CaptureShot);
		InputComponent->BindAction("OpenRecords", IE_Pressed, this, &UCaptureCamera::ToggleRecords);
		InputComponent->BindAction("RecordLeft",IE_Pressed, this, &UCaptureCamera::UpdateDetailsPrevious);
		InputComponent->BindAction("RecordRight", IE_Pressed, this, &UCaptureCamera::UpdateDetailsNext);
	}
	InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(InGameHUD){
		InGameHUD->CloseBook();
	} 
	CallNotification(false);

}


// Called every frame
void UCaptureCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Range; //Vector Calculation A + B = AB
	/*
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd, 
		FColor(0,255,0),
		false, 
		0.f,
		0,
		5.f
	); //Indicate the range of the player when interacting with the animals. 
	*/
	DetectAnimal();
	ProcessSighting();
	ShowCheckList();
}


void UCaptureCamera::CaptureShot(){
	UE_LOG(LogTemp, Warning, TEXT("SCREENSHOT CALLED!!!"));
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PController){
		PController->ConsoleCommand(TEXT("HighResShot 2"), true); 
	}
}

void UCaptureCamera::ToggleRecords(){
	if(InGameHUD){
			UE_LOG(LogTemp, Warning, TEXT("RECORD TOGGLING"));
		IsRecordViewable = !IsRecordViewable;
		if(IsRecordViewable){
			CallNotification(false);
			if(SeenAnimals.Num() == 0) InGameHUD->OpenInfo();
			else InGameHUD->OpenBook();		
		}
		else InGameHUD->CloseBook();
	}
}

void UCaptureCamera::DetectAnimal(){
	LineTraceEnd = GetPlayerLocation() + (GetPlayerRotation().Vector() * Range); //Vector() turns it into a unit vector. 
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); //GetOwner() to ensure that Character is not selected
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		OUT GetPlayerLocation(),
		OUT LineTraceEnd,
		OUT FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		OUT TraceParams
	);
	AnimalActor = Hit;
}

void UCaptureCamera::ProcessSighting(){
	AActor* ActorHit = AnimalActor.GetActor();
	if(ActorHit){
		TArray<FName> Tags = ActorHit->Tags;
		if(Tags.Contains("Animal")){ // || Tags.Contains("Plant") add if necessary
			TArray<FName> AnimalTag = Tags; 
			if(Tags.Contains("Animal"))AnimalTag.Remove("Animal");
			if(Tags.Contains("Plant"))AnimalTag.Remove("Plant");
			if(AnimalTag.Num() > 0){
				FName Identifier = AnimalTag.Pop(); //There are only two tags. First is the animal identifier, second is the animal. 
				if(!SeenAnimals.Contains(Identifier.ToString())){ 
					CallNotification(true);
					UE_LOG(LogTemp, Warning, TEXT("%s added to list of sighting entries!"), *Identifier.ToString());
					struct UBook::Species Specimen; 
					UBook::GetEntryFromTag(Identifier.ToString(), Specimen);
					SeenAnimals.Add(Identifier.ToString());
					SpeciesList.Add(Specimen);
					InGameHUD->RemoveAnimalFromList(Specimen.SpeciesName);
					if(SeenAnimals.Num() == 1) ProcessFirstAnimal();
				}
			}else{
				UE_LOG(LogTemp, Error, TEXT("An Associated Animal Tag has not been initialized with the %s object!"), *ActorHit->GetName());
			}
		}
	}
}

void UCaptureCamera::ProcessFirstAnimal(){
	if(InGameHUD){
		InGameHUD->DisplayAnimal(SpeciesList[0].SpeciesName,
									SpeciesList[0].SpeciesTag,
			 						SpeciesList[0].SpeciesDescription, 
									SpeciesList[0].SpeciesImageLink);
	}

}
void UCaptureCamera::CallNotification(bool Open){
	if(InGameHUD){
		if(Open) InGameHUD->DisplayNotification(); 	
		else InGameHUD->CloseNotification();
	}
}
void UCaptureCamera::ShowCheckList(){
	if(InGameHUD){
		InGameHUD->GenerateRemainingAnimals(); 
	}
}

FVector UCaptureCamera::GetPlayerLocation(){
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FRotator UCaptureCamera::GetPlayerRotation(){
		FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	return PlayerViewPointRotation;
}
//

void UCaptureCamera::UpdateDetailsNext(){
	if(IsRecordViewable && SpeciesList.Num() > 0 && (SpeciesList.Num() > Reference+1)){
		Reference++;
		if(InGameHUD){
			InGameHUD->DisplayAnimal(SpeciesList[Reference].SpeciesName,
									SpeciesList[Reference].SpeciesTag,
			 						SpeciesList[Reference].SpeciesDescription, 
									SpeciesList[Reference].SpeciesImageLink);
		}
	}
}

void UCaptureCamera::UpdateDetailsPrevious(){
	if(IsRecordViewable && SpeciesList.Num() > 0 && (Reference > 0)){
		Reference--;
		if(InGameHUD){
			InGameHUD->DisplayAnimal(SpeciesList[Reference].SpeciesName,
									SpeciesList[Reference].SpeciesTag,
			 						SpeciesList[Reference].SpeciesDescription, 
									SpeciesList[Reference].SpeciesImageLink);
		}
	}
}