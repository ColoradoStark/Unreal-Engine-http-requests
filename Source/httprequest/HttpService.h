// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "HttpService.generated.h"


USTRUCT(BlueprintType)
struct FRequest_Login {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite) FString email;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString password;

	FRequest_Login() {}
};

USTRUCT(BlueprintType)
struct FResponse_Login {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString hash;

	FResponse_Login() {}
};

USTRUCT(BlueprintType)
struct FResponse_GetPlayerById {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString playername;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString playerclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString weaponelement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int playerlevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int weaponlevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int hp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int mana = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int gold = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int strength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int dexterity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int intelligence = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int constitution = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int reg_date; //TODO This is the wrong type in PHP Change in PHP to time since epoch


	//FResponse_GetPlayerById() {} TODO: REMOVE ALL OF THESE CONSTRUCTORS
};

USTRUCT(BlueprintType)
struct FRequest_CreateNewPlayer {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString playername;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString playerclass;

	FRequest_CreateNewPlayer() {}
};

USTRUCT(BlueprintType)
struct FResponse_CreateNewPlayer {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString message;

	FResponse_CreateNewPlayer() {}
};

USTRUCT(BlueprintType)
struct FResponse_DeletePlayerById {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString message;

	FResponse_DeletePlayerById() {}
};

USTRUCT(BlueprintType)
struct FRequest_UpdatePlayer {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString playername;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString playerclass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString weaponelement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int playerlevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int weaponlevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int dexterity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int intelligence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int constitution;

	FRequest_UpdatePlayer() {}
};

USTRUCT(BlueprintType)
struct FResponse_UpdatePlayer {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString message;

	FResponse_UpdatePlayer() {}
};


USTRUCT(BlueprintType)
struct FPlovdivUniversity {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int students;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString teachers;
};





UCLASS()
class HTTPREQUEST_API AHttpService : public AActor
{
	GENERATED_BODY()

//private:
protected:
	FHttpModule* Http;
	FString ApiBaseUrl = "http://localhost:8080/";
	//FString ApiBaseUrl = "http://52.214.193.147/";

	FString AuthorizationHeader = TEXT("Authorization");
	void SetAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJsonString);
	TSharedRef<IHttpRequest> DeleteRequest(FString Subroute);
	TSharedRef<IHttpRequest> PutRequest(FString Subroute, FString ContentJsonString);
	void Send(TSharedRef<IHttpRequest>& Request);

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
	FResponse_Login Response_Login_Cache;

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	// EXERCISE: ADD CODE TO CREATE A Response_CreateNewPlayer_Cache Struct
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable)
	void Login(FRequest_Login LoginCredentials);
	void LoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateNewPlayer(FRequest_CreateNewPlayer PlayerCredentials);   
	void CreateNewPlayerResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetPlayerById(int PlayerID);
	void GetPlayerByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);  

	UFUNCTION(BlueprintCallable)
	void UpdatePlayer(FRequest_UpdatePlayer NewPlayerData);
	void UpdatePlayerResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DeletePlayerById(int PlayerID);
	void DeletePlayerByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
