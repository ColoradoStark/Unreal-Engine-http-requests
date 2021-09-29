// Copyright 2018 Colorado Stark

#include "HttpService.h"

AHttpService::AHttpService() { PrimaryActorTick.bCanEverTick = false; }
void AHttpService::BeginPlay() {
	Super::BeginPlay();
	Http = &FHttpModule::Get();

	FRequest_Create PlayerCredentials;
	PlayerCredentials.name = TEXT("Bilbo");
	PlayerCredentials.ip = TEXT("200.100.50.25");
	PlayerCredentials.hp = 100;
	PlayerCredentials.mana = 100;

	Create(PlayerCredentials);

	//FRequest_GetPlayerInfo PlayerToGet;
	//PlayerToGet.id = 1;
	//GetPlayerInfo(PlayerToGet);

}




/**************************************************************************************************************************/



TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHttpService::RequestWithRoute(FString Subroute) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void AHttpService::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHttpService::GetRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHttpService::PostRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHttpService::DeleteRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("DELETE");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHttpService::PutRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("PUT");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

void AHttpService::Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request) {
	Request->ProcessRequest();
}

bool AHttpService::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}

void AHttpService::SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request) {
	Request->SetHeader(AuthorizationHeader, Hash);
}



/**************************************************************************************************************************/



template <typename StructType>
void AHttpService::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput) {
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void AHttpService::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}



/**************************************************************************************************************************/



void AHttpService::Create(FRequest_Create CreateCredentials)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_Create>(CreateCredentials, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest("create/", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::CreateResponse);
	Send(Request);

	UE_LOG(LogTemp, Warning, TEXT("Create Request Called"));

}

void AHttpService::CreateResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_Create CreateResponse;
	GetStructFromJsonString<FResponse_Create>(Response, CreateResponse);

	Response_Create_Cache.id = CreateResponse.id;
	Response_Create_Cache.message = CreateResponse.message;

	UE_LOG(LogTemp, Warning, TEXT("Id is: %d"), CreateResponse.id);
	UE_LOG(LogTemp, Warning, TEXT("Message is: %s"), *CreateResponse.message);

}

/*

void AHttpService::GetPlayerInfo(FRequest_GetPlayerInfo PlayerID)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_GetPlayerInfo>(PlayerID, ContentJsonString);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest("getplayerinfo/", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::GetPlayerInfoResponse);
	Send(Request);

	UE_LOG(LogTemp, Warning, TEXT("Create Request Called"));

}

void AHttpService::GetPlayerInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_GetPlayerInfo GetPlayerInfoResponse;
	GetStructFromJsonString<FResponse_GetPlayerInfo>(Response, GetPlayerInfoResponse);

	Response_GetPlayerInfo_Cache.name = GetPlayerInfoResponse.name;
	Response_GetPlayerInfo_Cache.ip = GetPlayerInfoResponse.ip;
	Response_GetPlayerInfo_Cache.hp = GetPlayerInfoResponse.hp;
	Response_GetPlayerInfo_Cache.mana = GetPlayerInfoResponse.mana;

	UE_LOG(LogTemp, Warning, TEXT("Name is: %s"), *GetPlayerInfoResponse.name);
	UE_LOG(LogTemp, Warning, TEXT("IP is: %s"), *GetPlayerInfoResponse.ip);
	UE_LOG(LogTemp, Warning, TEXT("HP is: %d"), GetPlayerInfoResponse.hp);
	UE_LOG(LogTemp, Warning, TEXT("Mana is: %d"), GetPlayerInfoResponse.mana);
}


*/