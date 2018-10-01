// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpService.h"
#include "API_Client_Done.generated.h"

/**
 * 
 */
UCLASS()
class HTTPREQUEST_API AAPI_Client_Done : public AHttpService
{
	GENERATED_BODY()

public:

	FResponse_Login ResponseCache;

	UFUNCTION(BlueprintCallable)
	FResponse_Login FakeLoginReqest(FRequest_Login LoginCredentials);

	void TempLogin(FRequest_Login LoginCredentials);
	void TempLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);




	// Probably not necessary
	void Login(FRequest_Login LoginCredentials, FResponse_Login ResponseForBlueprint);


	
	
	
	
};
