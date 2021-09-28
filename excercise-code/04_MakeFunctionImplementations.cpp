void AHttpService::GetPlayerInfo(FRequest_Create GetPlayerInfoCredentials)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_Create>(GetPlayerInfoCredentials, ContentJsonString);

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
