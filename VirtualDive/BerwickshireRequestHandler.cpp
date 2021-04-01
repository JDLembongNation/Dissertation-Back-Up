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
		if(records.Num() > 0){
			val = "";
			val.AppendInt(records[0]->AsNumber());
			UE_LOG(LogTemp, Warning, TEXT("%s IS USED"), *val);
			TSharedPtr<FJsonObject> AnimalSpec = ItemObject->GetObjectField(val);
			if(AnimalSpec->HasField("squarethumbs")){
			TArray<TSharedPtr<FJsonValue>> ImageList = AnimalSpec->GetArrayField("squarethumbs");
			if(ImageList.Num()>0){
				FString url = ImageList[0]->AsString();
				ReferenceImage.Add(url, pair.Value);//URL and Tag name 
				CallForImage(url);
			}
			}
		}
	}
}

FString UBerwickshireRequestHandler::SplitString(FString input){
	FString newString = "";
	for(int32 i = 0; i < input.Len(); i+=60){
		if(input.Len()-1 >= i+60){
			newString+=input.Mid(i, 60);
			newString+="\n ";
			//add full
		}else{
			newString+=input.Mid(i, input.Len()-1);
			return newString;
		}
	}
	return newString;

}
void UBerwickshireRequestHandler::CallForImage(FString url){
	TSharedRef<IHttpRequest> ImageRequest = Http->CreateRequest();
	ImageRequest->SetVerb("GET");
	ImageRequest->SetURL(url);
	ImageRequest->OnProcessRequestComplete().BindUObject(this, &UBerwickshireRequestHandler::OnImageReceived);
	ImageRequest->ProcessRequest();
}

void UBerwickshireRequestHandler::OnImageReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful){
	if(bWasSuccessful && Response.IsValid()){
		TArray<uint8> ImageData = Response->GetContent();
		TSharedPtr<FSlateDynamicImageBrush> Image = CreateBrush(FName(*Request->GetURL()), ImageData); //have null check.
		UE_LOG(LogTemp, Warning, TEXT("Brush Created"));
		for(const TPair<FString, FString>&pair: ReferenceImage){
			if(pair.Key.Equals(Request->GetURL())){
				if(Image == NULL){
					Image = CreateLocalBrushes(pair.Value);
				}
				UBook::SpeciesImageDictionary.Add(pair.Value, Image);
				} 
		}
	}else{
		for(const TPair<FString, FString>&pair: ReferenceImage){
			if(pair.Key.Equals(Request->GetURL())){
				TSharedPtr<FSlateDynamicImageBrush> Image = CreateLocalBrushes(pair.Value);
				UBook::SpeciesImageDictionary.Add(pair.Value, Image);
			}	
		}
	}
}

TSharedPtr<FSlateDynamicImageBrush> UBerwickshireRequestHandler::CreateLocalBrushes(FString Tag){
		TSharedPtr<FSlateDynamicImageBrush> Brush;
	uint32 BytesPerPixel=4;
	int32 Width = 0;
	int32 Height = 0;
	bool bSucceeded = false;
	TArray<uint8> CompressedDataImage;
	TArray<uint8> DecodedImage;
	FString ImageName = Tag+"_Brush.jpeg";
	FString ImagePath = FPaths::ProjectContentDir() + "Images/" + ImageName;
	FFileHelper::LoadFileToArray(CompressedDataImage, *ImagePath);
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
     IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	 if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedDataImage.GetData(), CompressedDataImage.Num()))
     {
         Width = ImageWrapper->GetWidth();
         Height = ImageWrapper->GetHeight();

        TArray<uint8> RawData;
 
         if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawData))
         {
             DecodedImage = RawData;
             bSucceeded = true;
         }
     }
 
     if (bSucceeded && FSlateApplication::Get().GetRenderer()->GenerateDynamicImageResource(FName(Tag), ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), DecodedImage))
     {
         Brush = MakeShareable(new FSlateDynamicImageBrush(FName(Tag), FVector2D(ImageWrapper->GetWidth(), ImageWrapper->GetHeight())));
     }
 
     return Brush;
}
TSharedPtr<FSlateDynamicImageBrush> UBerwickshireRequestHandler::CreateBrush(FName ResourceName, TArray<uint8> ImageData){
	TSharedPtr<FSlateDynamicImageBrush> Brush;
	uint32 BytesPerPixel=4;
	int32 Width = 0;
	int32 Height = 0;
	bool bSucceeded = false;
     TArray<uint8> DecodedImage;
     IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
     IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
 
     if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
     {
         Width = ImageWrapper->GetWidth();
         Height = ImageWrapper->GetHeight();
 
        TArray<uint8> RawData;
 
         if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawData))
         {
             DecodedImage = RawData;
             bSucceeded = true;
         }
     }
 
     if (bSucceeded && FSlateApplication::Get().GetRenderer()->GenerateDynamicImageResource(ResourceName, ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), DecodedImage))
     {
         Brush = MakeShareable(new FSlateDynamicImageBrush(ResourceName, FVector2D(ImageWrapper->GetWidth(), ImageWrapper->GetHeight())));
     }
 
     return Brush;

}

void UBerwickshireRequestHandler::CreateMap(){
	ReferenceMap.Add(68, "Seal"); //Grey Seals
	ReferenceMap.Add(90, "Ray"); //Grey Seals
	ReferenceMap.Add(4312,"Whale"); //Grey Seals
	ReferenceMap.Add(4310,"Dolphin"); //Grey Seals
	ReferenceMap.Add(46,"Wolffish"); //Grey Seals
	ReferenceMap.Add(61,"Crab"); //Grey Seals
	ReferenceMap.Add(48,"Lobster"); //Grey Seals
	ReferenceMap.Add(51,"Jellyfish"); //Grey Seals
	ReferenceMap.Add(423, "Kelp"); //Need to be targeted a different way. 
}
