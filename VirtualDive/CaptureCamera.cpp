// Fill out your copyright notice in the Description page of Project Settings.
/*
	ATTACH THIS ACTOR COMPONENT TO THE MAIN CHARACTER IN THE SCENE.
*/

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
	//bind all inputs to appropriate methods
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		InputComponent->BindAction("CaptureScreenshot",IE_Pressed, this, &UCaptureCamera::CaptureShot);
		InputComponent->BindAction("OpenRecords", IE_Pressed, this, &UCaptureCamera::ToggleRecords);
		InputComponent->BindAction("RecordLeft",IE_Pressed, this, &UCaptureCamera::UpdateDetailsPrevious);
		InputComponent->BindAction("RecordRight", IE_Pressed, this, &UCaptureCamera::UpdateDetailsNext);
		InputComponent->BindAction("ToggleInstructions", IE_Pressed, this, &UCaptureCamera::ToggleInstructions);
		InputComponent->BindAction("Exit", IE_Pressed, this, &UCaptureCamera::QuitGame);
	}
	InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(InGameHUD){
		InGameHUD->CloseBook();
	} 
	CallNotification(false);
	ToggleInstructions();

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
	DisplayArrows();
}

//METHOD for toggling instructions from the dedicated input key.
void UCaptureCamera::ToggleInstructions(){
	if(InGameHUD){
	IsInstructionsClosed = !IsInstructionsClosed;
	if(!IsInstructionsClosed){
		InGameHUD->OpenInstructionPanel();
	}else{
		InGameHUD->CloseInstructionPanel();
	}
	}
}

//METHOD for activating screenshot from the dedicated input key.
void UCaptureCamera::CaptureShot(){
	UE_LOG(LogTemp, Warning, TEXT("Screenshot Captured"));
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PController){
		PController->ConsoleCommand(TEXT("HighResShot 2"), true); 
	}
}

//METHOD for toggling record HUD from the dedicated input key.
void UCaptureCamera::ToggleRecords(){
	if(InGameHUD){
		IsRecordViewable = !IsRecordViewable;
		if(IsRecordViewable){
			CallNotification(false);
			if(SeenAnimals.Num() == 0) InGameHUD->OpenInfo();
			else{
				InGameHUD->OpenBook();
			} 		
		}
		else InGameHUD->CloseBook();
	}
}

//METHOD for displaying the arrows in the Record HUD.
void UCaptureCamera::DisplayArrows(){
	if(InGameHUD && Reference >=0 && IsRecordViewable){
		if(Reference > 0){
			InGameHUD->ActivateLeftArrow();
		}else{
			InGameHUD->DisableLeftArrow();
		}
		if(Reference < SpeciesList.Num()-1){
			InGameHUD->ActivateRightArrow();
		}else{
			InGameHUD->DisableRightArrow();
		}
	}
}

//METHOD for capturing any actor that the raycast from the player hits.
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

//METHOD for processing the tags of the actors caught and adding them into the appropriate data stores.
void UCaptureCamera::ProcessSighting(){
	AActor* ActorHit = AnimalActor.GetActor();
	if(ActorHit){
		TArray<FName> Tags = ActorHit->Tags;
		if(Tags.Contains("Animal")){ 
			TArray<FName> AnimalTag = Tags; 
			AnimalTag.Remove("Animal");
			if(AnimalTag.Num() > 0){
				FName Identifier = AnimalTag.Pop(); //There are only two tags. First is the animal identifier, second is the animal. 
				if(!SeenAnimals.Contains(Identifier.ToString())){ 
					if (!IsRecordViewable) {
						CallNotification(true);
					}
					UE_LOG(LogTemp, Warning, TEXT("%s added to list of sighting entries!"), *Identifier.ToString());
					struct UBook::Species Specimen; 
					UBook::GetEntryFromTag(Identifier.ToString(), Specimen);
					SeenAnimals.Add(Identifier.ToString());
					SpeciesList.Add(Specimen);
					InGameHUD->RemoveAnimalFromList(Specimen.SpeciesName);
					if(SeenAnimals.Num() == 1){
						InGameHUD->CloseInfo();
						ProcessFirstAnimal();	
					}else{
						if(IsRecordViewable){
							UpdateDetailsNext();
						}else{
							ShowNextAnimal();
						}
					}
				}
			}else{
				UE_LOG(LogTemp, Error, TEXT("An Associated Animal Tag has not been initialized with the %s object!"), *ActorHit->GetName());
			}
		}
	}
}

void UCaptureCamera::ProcessFirstAnimal(){
	if(InGameHUD){
		Reference++;
		InGameHUD->DisplayAnimal(SpeciesList[Reference].SpeciesName,
									SpeciesList[Reference].SpeciesTag,
			 						SpeciesList[Reference].SpeciesDescription, 
									SpeciesList[Reference].SpeciesImageLink);
		if(IsRecordViewable){
			InGameHUD->OpenBook();
		}
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
void UCaptureCamera::ShowNextAnimal(){
	if(InGameHUD){
		Reference++;
		InGameHUD->DisplayAnimal(SpeciesList[Reference].SpeciesName,
						SpeciesList[Reference].SpeciesTag,
						SpeciesList[Reference].SpeciesDescription, 
						SpeciesList[Reference].SpeciesImageLink);
	}
}

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
void UCaptureCamera::QuitGame(){
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PController){	
		PController->ConsoleCommand(TEXT("exit")); 
	}
}