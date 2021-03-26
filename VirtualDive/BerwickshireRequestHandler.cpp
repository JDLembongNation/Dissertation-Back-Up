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
	TSharedPtr<FJsonObject> AnimalCollection  = JsonObject->GetObjectField("collections");
	TSharedPtr<FJsonObject> ItemObject  = JsonObject->GetObjectField("items");
	TSharedPtr<FJsonObject> CollectionItemObject = JsonObject->GetObjectField("collectionItems");
	for(const TPair<int32, FString>&pair: ReferenceMap){
		FString val = "";
		val.AppendInt(pair.Key);
		TSharedPtr<FJsonObject> Animal  = AnimalCollection->GetObjectField(val);
		FString identifier = Animal->GetStringField("Identifier");
		int id = Animal->GetIntegerField("id");
		TArray<TSharedPtr<FJsonValue>> records = CollectionItemObject->GetArrayField(val);
		TSharedPtr<FJsonObject> value = ItemObject->GetObjectField(identifier);
		struct UBook::Species Specimen; 
		Specimen.SpeciesName = value->GetStringField("Title");
		if(value->HasField("Description")){
			FString description = SplitString(value->GetStringField("Description"));
			Specimen.SpeciesDescription = description;
		} 
		else Specimen.SpeciesDescription = TEXT("None");
		//Specimen.SpeciesImageLink = value->GetStringField("Title");
		Specimen.SpeciesTag = pair.Value;
		UBook::SpeciesDictionary.Add(pair.Value, Specimen); //Tag and then species
	}
}
FString UBerwickshireRequestHandler::SplitString(FString input){
	FString newString = "";
	for(int32 i = 0; i < input.Len(); i+=70){
		if(input.Len()-1 >= i+70){
			newString+=input.Mid(i, 70);
			newString+="\n ";
			//add full
		}else{
			newString+=input.Mid(i, input.Len()-1);
			return newString;
		}
	}
	return newString;

}

void UBerwickshireRequestHandler::CreateMap(){
	ReferenceMap.Add(68, "Seal"); //Grey Seals
	ReferenceMap.Add(90, "Ray"); //Grey Seals
	ReferenceMap.Add(4312,"Whale"); //Grey Seals
	ReferenceMap.Add(46,"Wolffish"); //Grey Seals
	ReferenceMap.Add(61,"Crab"); //Grey Seals
	ReferenceMap.Add(48,"Lobster"); //Grey Seals
	ReferenceMap.Add(51,"Jellyfish"); //Grey Seals
}