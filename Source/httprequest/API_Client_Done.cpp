// Fill out your copyright notice in the Description page of Project Settings.

#include "API_Client_Done.h"





FResponse_Login AAPI_Client_Done::FakeLoginReqest(FRequest_Login LoginCredentials)
{

	FResponse_Login LoginResponse;


	return LoginResponse;
}



void AAPI_Client_Done::TempLogin(FRequest_Login LoginCredentials)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_Login>(LoginCredentials, ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("user/login", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AAPI_Client_Done::TempLoginResponse);
	Send(Request);
}

void AAPI_Client_Done::TempLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

}




/////////Probably don't need this

void AAPI_Client_Done::Login(FRequest_Login LoginCredentials, FResponse_Login ResponseForBlueprint)
{
	FString ContentJsonString;
	GetJsonStringFromStruct<FRequest_Login>(LoginCredentials, ContentJsonString);

	TSharedRef<IHttpRequest> Request = PostRequest("user/login", ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpService::LoginResponse);
	Send(Request);
}


