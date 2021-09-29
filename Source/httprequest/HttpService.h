// Copyright 2018 Colorado Stark

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "HttpService.generated.h"

USTRUCT(BlueprintType)
struct FPlovdivUniversity {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int students;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString teachers;
};

USTRUCT(BlueprintType)
struct FRequest_Create {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int mana;

	FRequest_Create() {}
};

USTRUCT(BlueprintType)
struct FResponse_Create {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString message;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;	

	FResponse_Create() {}
};


/*
USTRUCT(BlueprintType)
struct FRequest_GetPlayerInfo {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;

	FRequest_GetPlayerInfo() {}
};

USTRUCT(BlueprintType)
struct FResponse_GetPlayerInfo {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int mana;

	FResponse_GetPlayerInfo() {}
};

*/


UCLASS()
class HTTPREQUEST_API AHttpService : public AActor
{
	GENERATED_BODY()

//private:
protected:
	FHttpModule* Http;
	FString ApiBaseUrl = "http://75b1-35-196-122-164.ngrok.io/";

	FString AuthorizationHeader = TEXT("Authorization");
	void SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GetRequest(FString Subroute, FString ContentJsonString);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PostRequest(FString Subroute, FString ContentJsonString);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> DeleteRequest(FString Subroute, FString ContentJsonString);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PutRequest(FString Subroute, FString ContentJsonString);
	void Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);

	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
	
public:	
	// Sets default values for this actor's properties
	AHttpService();
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	FResponse_Create Response_Create_Cache;

	/*
	UPROPERTY(BlueprintReadWrite)
	FResponse_GetPlayerInfo Response_GetPlayerInfo_Cache;
	*/
	

	UFUNCTION(BlueprintCallable)
	void Create(FRequest_Create CreateCredentials);
	void CreateResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*
	UFUNCTION(BlueprintCallable)
	void GetPlayerInfo(FRequest_GetPlayerInfo PlayerID);
	void GetPlayerInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	*/
	
};
