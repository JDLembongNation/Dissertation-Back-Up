// Fill out your copyright notice in the Description page of Project Settings.


#include "BerwickshireRequestHandler.h"

// Sets default values for this component's properties
UBerwickshireRequestHandler::UBerwickshireRequestHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Http = &FHttpModule::Get();
	// ...
}


// Called when the game starts
void UBerwickshireRequestHandler::BeginPlay()
{
	Super::BeginPlay();
	CreateMap();
	CallToDatabase();
	// ...
	
}


// Called every frame
void UBerwickshireRequestHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UBerwickshireRequestHandler::CallToDatabase()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UBerwickshireRequestHandler::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL("https://berwickshiremarinereserve.uk/map/dataJSON.php");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}
/*Assigned function on successfull http call*/
void UBerwickshireRequestHandler::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response.IsValid()){
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject)){
		UE_LOG(LogTemp, Warning, TEXT("SUCCESSFULLY CONNECTED"));
		ProcessJSON(JsonObject);
	}
	FString path = FPaths::ProjectContentDir() + "db/contents.json";
	FFileHelper::SaveStringToFile(Response->GetContentAsString(), *path);

	}else{
		const FString JsonFilePath = FPaths::ProjectContentDir() + "db/contents.json";
		FString JsonString; //Json converted to FString
	
		FFileHelper::LoadFileToString(JsonString,*JsonFilePath);
			TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("SUCCESSFULLY PUlled from Local Database"));
		ProcessJSON(JsonObject);
	}
	}
}
void UBerwickshireRequestHandler::ProcessJSON(TSharedPtr<FJsonObject> JsonObject){
	TSharedPtr<FJsonObject> ItemObject  = JsonObject->GetObjectField("items");
	for(const TPair<int32, FString>&pair: ReferenceMap){
		FString result ="";
		result.AppendInt(pair.Key);
		TSharedPtr<FJsonObject> value = ItemObject->GetObjectField(result);
		struct UBook::Species Specimen; 
		Specimen.SpeciesName = value->GetStringField("Title");
		//Specimen.SpeciesDescription = value->GetStringField("Title");
		//Specimen.SpeciesImageLink = value->GetStringField("Title");
		Specimen.SpeciesTag = pair.Value;
		UBook::SpeciesDictionary.Add(pair.Value, Specimen); //Tag and then species
	}
}

void UBerwickshireRequestHandler::CreateMap(){
	ReferenceMap.Add(224, "Seal"); //Grey Seals
	ReferenceMap.Add(4651, "Dolphin");
	ReferenceMap.Add(246, "Ray");
	ReferenceMap.Add(4653, "Whale");
	ReferenceMap.Add(330, "Lobster");
	ReferenceMap.Add(228, "Crab");
	ReferenceMap.Add(202, "Wolffish");
	ReferenceMap.Add(207, "Jellyfish");
}