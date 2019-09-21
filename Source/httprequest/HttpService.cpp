// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpService.h"

AHttpService::AHttpService() { PrimaryActorTick.bCanEverTick = false; }
void AHttpService::BeginPlay() {
	Super::BeginPlay();
	Http = &FHttpModule::Get();

	// You can uncomment these sections to test.

	//////////////////////////////////////////////////////////
	// LOGIN

	//FRequest_Login LoginCredentials;
	//LoginCredentials.email = TEXT("asdf@asdf.com");
	//LoginCredentials.password = TEXT("asdfasdf");
	//Login(LoginCredentials);

	//////////////////////////////////////////////////////////
	// CREATE
	
	//FRequest_CreateNewPlayer PlayerCredentials;
	//PlayerCredentials.playername = TEXT("Navajo");
	//PlayerCredentials.playerclass = TEXT("wizard");
	//CreateNewPlayer(PlayerCredentials);

	//////////////////////////////////////////////////////////
	// READ

	GetPlayerById(2);

	//////////////////////////////////////////////////////////
	// UPDATE

	/*
	FRequest_UpdatePlayer NewPlayerData;
	NewPlayerData.id = 4;
	NewPlayerData.playername = TEXT("Sequoyah");
	NewPlayerData.playerclass = TEXT("wizard");
	NewPlayerData.weapon = TEXT("staff");
	NewPlayerData.weaponelement = TEXT("none"); 
	NewPlayerData.playerlevel = 101;
	NewPlayerData.weaponlevel = 0;
	NewPlayerData.hp = 20;
	NewPlayerData.mana = 20;
	NewPlayerData.gold = 100;
	NewPlayerData.strength = 9;
	NewPlayerData.dexterity = 9;
	NewPlayerData.intelligence = 9;
	NewPlayerData.constitution = 9;

	UpdatePlayer(NewPlayerData);
	*/

	//////////////////////////////////////////////////////////
	// DELETE

	//DeletePlayerById(9);
}




/**************************************************************************************************************************/



TSharedRef<IHttpRequest> AHttpService::RequestWithRoute(FString Subroute) {
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void AHttpService::SetRequestHeaders(TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> AHttpService::GetRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest> AHttpService::PostRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

TSharedRef<IHttpRequest> AHttpService::DeleteRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("DELETE");
	return Request;
}

TSharedRef<IHttpRequest> AHttpService::PutRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("PUT");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

void AHttpService::Send(TSharedRef<IHttpRequest>& Request) {
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

void AHttpService::SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request) {
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



void AHttpService::Login(FRequest_Login LoginCredentials) {
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_Login>(LoginCredentials, ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("user/login", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::LoginResponse);
	Send(Request);
}

void AHttpService::LoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_Login LoginResponse;
	GetStructFromJsonString<FResponse_Login>(Response, LoginResponse);

	Response_Login_Cache.id = LoginResponse.id;
	Response_Login_Cache.name = LoginResponse.name;
	Response_Login_Cache.hash = LoginResponse.hash;

	UE_LOG(LogTemp, Warning, TEXT("Id is: %d"), LoginResponse.id);
	UE_LOG(LogTemp, Warning, TEXT("Name is: %s"), *LoginResponse.name);
}



void AHttpService::CreateNewPlayer(FRequest_CreateNewPlayer PlayerCredentials) {
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_CreateNewPlayer>(PlayerCredentials, ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("api/createnewplayer", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::CreateNewPlayerResponse);
	Send(Request);
}

void AHttpService::CreateNewPlayerResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_CreateNewPlayer CreateNewPlayerResponse;
	GetStructFromJsonString<FResponse_CreateNewPlayer>(Response, CreateNewPlayerResponse);

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	// EXERCISE: ADD CODE TO SET Response_CreateNewPlayer_Cache
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	UE_LOG(LogTemp, Warning, TEXT("Result of operation to create player was: %s"), *CreateNewPlayerResponse.message);
	UE_LOG(LogTemp, Warning, TEXT("Your Newly Created Player ID is: %d"), CreateNewPlayerResponse.id);
}

void AHttpService::GetPlayerById(int PlayerId) {

	FString subroute("api/getplayerbyid/");
	FString id = FString::FromInt(PlayerId);
	subroute.Append(id);

	//UE_LOG(LogTemp, Warning, TEXT("The subroute path is: %s"), *subroute);
	
	

	TSharedRef<IHttpRequest> Request = GetRequest(subroute);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::GetPlayerByIdResponse);
	Send(Request);
}

void AHttpService::GetPlayerByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_GetPlayerById GetPlayerByIdResponse;
	GetStructFromJsonString<FResponse_GetPlayerById>(Response, GetPlayerByIdResponse);

	UE_LOG(LogTemp, Warning, TEXT("You retrieved player ID number: %d"), GetPlayerByIdResponse.id);
	UE_LOG(LogTemp, Warning, TEXT("The name of this player is : %s"), *GetPlayerByIdResponse.playername);
}

void AHttpService::UpdatePlayer(FRequest_UpdatePlayer NewPlayerData) {
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_UpdatePlayer>(NewPlayerData, ContentJsonString);

	TSharedRef<IHttpRequest> Request = PutRequest("api/updateplayer", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::UpdatePlayerResponse);
	Send(Request);
}

void AHttpService::UpdatePlayerResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_UpdatePlayer UpdatePlayerResponse;
	GetStructFromJsonString<FResponse_UpdatePlayer>(Response, UpdatePlayerResponse);

	UE_LOG(LogTemp, Warning, TEXT("Result of operation to update player was: %s"), *UpdatePlayerResponse.message);
	UE_LOG(LogTemp, Warning, TEXT("You Updated Player ID: %d"), UpdatePlayerResponse.id);
}


void AHttpService::DeletePlayerById(int PlayerId) {

	FString subroute("api/deleteplayerbyid/");
	FString id = FString::FromInt(PlayerId);
	subroute.Append(id);

	//UE_LOG(LogTemp, Warning, TEXT("The subroute path is: %s"), *subroute);

	TSharedRef<IHttpRequest> Request = DeleteRequest(subroute);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::DeletePlayerByIdResponse);
	Send(Request);
}

void AHttpService::DeletePlayerByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	FResponse_DeletePlayerById DeletePlayerByIdResponse;
	GetStructFromJsonString<FResponse_DeletePlayerById>(Response, DeletePlayerByIdResponse);

	UE_LOG(LogTemp, Warning, TEXT("The result of the delete operation was: %d"), DeletePlayerByIdResponse.id);
	UE_LOG(LogTemp, Warning, TEXT("The number of the deleted player is : %s"), *DeletePlayerByIdResponse.message);
}
